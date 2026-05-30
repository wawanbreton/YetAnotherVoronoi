// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <memory>
#include <limits>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/core/access.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "yav/generator/bisector/VertexVertexBisectorGenerator.h"
#include "yav/geometry/Point3.h"
#include "yav/geometry/Point3Operations.h"
#include "yav/space/Primitive.h"
#include "yav/space/Space.h"
#include "yav/space/site/AbstractSite.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/Cell.h"
#include "yav/voronoi/CellPatch.h"
#include "yav/voronoi/Diagram.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

namespace yav::generator
{

namespace
{

constexpr double kDeterminantEpsilon = 1.0e-9;
constexpr double kContainmentEpsilon = 1.0e-8;

struct VertexPrimitive
{
    std::size_t index;
    std::shared_ptr<space::Primitive> primitive;
    std::shared_ptr<space::site::Vertex> site;
};

struct SweepEvent
{
    enum class Kind
    {
        Site,
        Vertex,
    };

    Kind kind;
    double sweep_coordinate;
    std::vector<std::size_t> site_indices;
    std::optional<geometry::Point3> sphere_center;
    double sphere_radius = 0.0;
};

struct PatchPair
{
    std::shared_ptr<voronoi::CellPatch> first;
    std::shared_ptr<voronoi::CellPatch> second;
};

double squaredNorm(const geometry::Point3& value)
{
    return boost::geometry::dot_product(value, value);
}

geometry::Point3 subtract(const geometry::Point3& lhs, const geometry::Point3& rhs)
{
    return geometry::Point3(
        boost::geometry::get<0>(lhs) - boost::geometry::get<0>(rhs),
        boost::geometry::get<1>(lhs) - boost::geometry::get<1>(rhs),
        boost::geometry::get<2>(lhs) - boost::geometry::get<2>(rhs));
}

double determinant3x3(const std::array<std::array<double, 3>, 3>& matrix)
{
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
           - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
           + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

std::optional<geometry::Point3> solveLinearSystem3x3(
    const std::array<std::array<double, 3>, 3>& matrix,
    const std::array<double, 3>& rhs)
{
    const double determinant = determinant3x3(matrix);
    if (std::abs(determinant) <= kDeterminantEpsilon)
    {
        return std::nullopt;
    }

    auto replace_column = [&matrix](const std::array<double, 3>& values, const std::size_t column)
    {
        auto replaced = matrix;
        for (std::size_t row = 0; row < 3; ++row)
        {
            replaced[row][column] = values[row];
        }
        return replaced;
    };

    return geometry::Point3(
        determinant3x3(replace_column(rhs, 0)) / determinant,
        determinant3x3(replace_column(rhs, 1)) / determinant,
        determinant3x3(replace_column(rhs, 2)) / determinant);
}

std::optional<std::pair<geometry::Point3, double>> computeCircumsphere(
    const std::array<std::shared_ptr<space::site::Vertex>, 4>& sites)
{
    const geometry::Point3& first_point = sites[0]->position();

    std::array<std::array<double, 3>, 3> matrix {};
    std::array<double, 3> rhs {};

    for (std::size_t row = 0; row < 3; ++row)
    {
        const geometry::Point3 delta = subtract(sites[row + 1]->position(), first_point);
        matrix[row] = {
            2.0 * boost::geometry::get<0>(delta),
            2.0 * boost::geometry::get<1>(delta),
            2.0 * boost::geometry::get<2>(delta),
        };
        rhs[row] = squaredNorm(sites[row + 1]->position()) - squaredNorm(first_point);
    }

    const auto center = solveLinearSystem3x3(matrix, rhs);
    if (! center)
    {
        return std::nullopt;
    }

    const double radius = geometry::Point3Operations::norm(subtract(*center, first_point));
    return std::make_pair(*center, radius);
}

bool sphereIsEmpty(
    const geometry::Point3& center,
    const double radius,
    const std::vector<VertexPrimitive>& supported_vertices,
    const std::set<std::size_t>& excluded_indices)
{
    const double squared_radius = radius * radius;

    for (const auto& candidate : supported_vertices)
    {
        if (excluded_indices.contains(candidate.index))
        {
            continue;
        }

        const double squared_distance = squaredNorm(subtract(candidate.site->position(), center));
        if (squared_distance < squared_radius - kContainmentEpsilon)
        {
            return false;
        }
    }

    return true;
}

std::string makeBeachConstraint(
    const space::site::Vertex& site,
    const double sweep_coordinate,
    const voronoi::equisurface::Paraboloid& paraboloid)
{
    return fmt::format(
        "fortune.beach focus={} sweep_z={} scale={} offset={}",
        site.position(),
        sweep_coordinate,
        paraboloid.scale(),
        paraboloid.offset());
}

double beachHeightAt(const voronoi::equisurface::Paraboloid& paraboloid, const geometry::Point3& position)
{
    const double delta_x = boost::geometry::get<0>(position) - boost::geometry::get<0>(paraboloid.focus());
    const double delta_y = boost::geometry::get<1>(position) - boost::geometry::get<1>(paraboloid.focus());
    return paraboloid.scale() * (delta_x * delta_x + delta_y * delta_y)
           + boost::geometry::get<2>(paraboloid.focus())
           + paraboloid.offset();
}

std::string makeSiteConstraint(const std::size_t site_index, const double sweep_coordinate)
{
    return fmt::format("fortune.site-event site={} sweep_z={}", site_index, sweep_coordinate);
}

std::string makeVertexConstraint(
    const geometry::Point3& center,
    const double radius,
    const double sweep_coordinate,
    const std::vector<std::size_t>& site_indices)
{
    std::ostringstream stream;
    stream << "fortune.vertex-event sites=";
    for (std::size_t offset = 0; offset < site_indices.size(); ++offset)
    {
        if (offset != 0)
        {
            stream << ',';
        }
        stream << site_indices[offset];
    }

    stream << " center=" << fmt::format("{}", center);
    stream << " radius=" << radius;
    stream << " sweep_z=" << sweep_coordinate;
    return stream.str();
}

std::pair<std::size_t, std::size_t> makeKey(const std::size_t lhs, const std::size_t rhs)
{
    return lhs < rhs ? std::make_pair(lhs, rhs) : std::make_pair(rhs, lhs);
}

} // namespace

Generator::Generator()
    : bisector_generators_({ std::make_shared<bisector::VertexVertexBisectorGenerator>() })
{
}

voronoi::Diagram Generator::generate(const space::Space& input_space) const
{
    voronoi::Diagram output_diagram;
    std::map<std::size_t, std::shared_ptr<voronoi::Cell>> cells_by_index;
    std::vector<VertexPrimitive> supported_vertices;

    const auto& primitives = input_space.primitives();
    supported_vertices.reserve(primitives.size());

    for (std::size_t primitive_index = 0; primitive_index < primitives.size(); ++primitive_index)
    {
        const auto& primitive = primitives[primitive_index];
        if (! primitive)
        {
            spdlog::error("Encountered null primitive while generating Voronoi cells");
            continue;
        }

        auto cell = std::make_shared<voronoi::Cell>(primitive);
        output_diagram.addCell(cell);
        cells_by_index.emplace(primitive_index, cell);

        std::shared_ptr<space::site::Vertex> vertex_site;
        bool unsupported_site_kind = false;

        for (const auto& site : primitive->sites())
        {
            if (! site)
            {
                unsupported_site_kind = true;
                break;
            }

            if (site->siteKind() != space::site::SiteKind::Vertex)
            {
                unsupported_site_kind = true;
                break;
            }

            const auto cast_vertex = std::dynamic_pointer_cast<space::site::Vertex>(site);
            if (! cast_vertex)
            {
                unsupported_site_kind = true;
                break;
            }

            if (vertex_site)
            {
                unsupported_site_kind = true;
                break;
            }

            vertex_site = cast_vertex;
        }

        if (! vertex_site)
        {
            unsupported_site_kind = true;
        }

        if (unsupported_site_kind)
        {
            spdlog::warn(
                "Skipping primitive {} because the 3D Fortune sweep currently supports only primitives with a single vertex site",
                primitive_index);
            continue;
        }

        supported_vertices.push_back({ primitive_index, primitive, vertex_site });
    }

    if (supported_vertices.size() < 2)
    {
        spdlog::info("Need at least two supported vertex primitives to build a Voronoi diagram");
        return output_diagram;
    }

    std::vector<SweepEvent> events;
    events.reserve(supported_vertices.size());

    for (const auto& vertex_primitive : supported_vertices)
    {
        events.push_back({
            SweepEvent::Kind::Site,
            boost::geometry::get<2>(vertex_primitive.site->position()),
            { vertex_primitive.index },
            std::nullopt,
            0.0,
        });
    }

    if (supported_vertices.size() == 2)
    {
        const auto key = makeKey(supported_vertices[0].index, supported_vertices[1].index);
        auto first_cell = cells_by_index.at(key.first);
        auto second_cell = cells_by_index.at(key.second);
        const auto generated_bisector = generateBisector(supported_vertices[0].site, supported_vertices[1].site);

        if (generated_bisector && first_cell && second_cell)
        {
            auto first_patch = std::make_shared<voronoi::CellPatch>();
            auto second_patch = std::make_shared<voronoi::CellPatch>();
            first_patch->setBisector(generated_bisector);
            second_patch->setBisector(generated_bisector);
            first_patch->addAdjacentPatch(second_patch);
            second_patch->addAdjacentPatch(first_patch);
            first_patch->addConstraint("fortune.degenerate pairwise fallback");
            second_patch->addConstraint("fortune.degenerate pairwise fallback");
            first_cell->addPatch(first_patch);
            second_cell->addPatch(second_patch);
        }

        return output_diagram;
    }

    for (std::size_t first_index = 0; first_index + 3 < supported_vertices.size(); ++first_index)
    {
        for (std::size_t second_index = first_index + 1; second_index + 2 < supported_vertices.size(); ++second_index)
        {
            for (std::size_t third_index = second_index + 1; third_index + 1 < supported_vertices.size(); ++third_index)
            {
                for (std::size_t fourth_index = third_index + 1; fourth_index < supported_vertices.size(); ++fourth_index)
                {
                    const auto quadruple = std::array<std::shared_ptr<space::site::Vertex>, 4> {
                        supported_vertices[first_index].site,
                        supported_vertices[second_index].site,
                        supported_vertices[third_index].site,
                        supported_vertices[fourth_index].site,
                    };

                    const auto circumsphere = computeCircumsphere(quadruple);
                    if (! circumsphere)
                    {
                        continue;
                    }

                    const auto& [center, radius] = *circumsphere;
                    const std::set<std::size_t> excluded_indices {
                        supported_vertices[first_index].index,
                        supported_vertices[second_index].index,
                        supported_vertices[third_index].index,
                        supported_vertices[fourth_index].index,
                    };
                    if (! sphereIsEmpty(center, radius, supported_vertices, excluded_indices))
                    {
                        continue;
                    }

                    events.push_back({
                        SweepEvent::Kind::Vertex,
                        boost::geometry::get<2>(center) - radius,
                        {
                            supported_vertices[first_index].index,
                            supported_vertices[second_index].index,
                            supported_vertices[third_index].index,
                            supported_vertices[fourth_index].index,
                        },
                        center,
                        radius,
                    });
                }
            }
        }
    }

    std::ranges::sort(
        events,
        [](const SweepEvent& lhs, const SweepEvent& rhs)
        {
            if (lhs.sweep_coordinate != rhs.sweep_coordinate)
            {
                return lhs.sweep_coordinate > rhs.sweep_coordinate;
            }

            return lhs.kind == SweepEvent::Kind::Site && rhs.kind == SweepEvent::Kind::Vertex;
        });

    std::set<std::size_t> active_sites;
    std::map<std::size_t, voronoi::equisurface::Paraboloid> beach_surface;
    std::map<std::pair<std::size_t, std::size_t>, PatchPair> patch_pairs;

    auto find_supported = [&supported_vertices](const std::size_t site_index) -> const VertexPrimitive*
    {
        const auto iterator = std::ranges::find(
            supported_vertices,
            site_index,
            [](const VertexPrimitive& candidate)
            {
                return candidate.index;
            });
        return iterator == supported_vertices.end() ? nullptr : &*iterator;
    };

    auto ensure_patch_pair = [this, &cells_by_index, &patch_pairs, &find_supported](
                                 const std::size_t first_index,
                                 const std::size_t second_index) -> PatchPair*
    {
        const auto key = makeKey(first_index, second_index);
        auto iterator = patch_pairs.find(key);
        if (iterator != patch_pairs.end())
        {
            return &iterator->second;
        }

        const VertexPrimitive* first_vertex = find_supported(key.first);
        const VertexPrimitive* second_vertex = find_supported(key.second);
        if (! first_vertex || ! second_vertex)
        {
            return nullptr;
        }

        const auto bisector = generateBisector(first_vertex->site, second_vertex->site);
        if (! bisector)
        {
            return nullptr;
        }

        auto first_cell = cells_by_index.at(key.first);
        auto second_cell = cells_by_index.at(key.second);
        if (! first_cell || ! second_cell)
        {
            return nullptr;
        }

        PatchPair pair {
            std::make_shared<voronoi::CellPatch>(),
            std::make_shared<voronoi::CellPatch>(),
        };
        pair.first->setBisector(bisector);
        pair.second->setBisector(bisector);
        pair.first->addAdjacentPatch(pair.second);
        pair.second->addAdjacentPatch(pair.first);

        first_cell->addPatch(pair.first);
        second_cell->addPatch(pair.second);

        auto [inserted_iterator, inserted] = patch_pairs.emplace(key, std::move(pair));
        return inserted ? &inserted_iterator->second : &inserted_iterator->second;
    };

    auto patch_for_site = [&patch_pairs](const std::size_t first_index, const std::size_t second_index, const std::size_t target_index)
        -> std::shared_ptr<voronoi::CellPatch>
    {
        const auto key = makeKey(first_index, second_index);
        const auto iterator = patch_pairs.find(key);
        if (iterator == patch_pairs.end())
        {
            return nullptr;
        }

        return target_index == key.first ? iterator->second.first : iterator->second.second;
    };

    for (const auto& event : events)
    {
        if (event.kind == SweepEvent::Kind::Site)
        {
            const VertexPrimitive* activated_site = find_supported(event.site_indices.front());
            if (! activated_site)
            {
                continue;
            }

            active_sites.insert(activated_site->index);
            const double site_z = boost::geometry::get<2>(activated_site->site->position());
            const double denominator = 2.0 * (site_z - event.sweep_coordinate + kDeterminantEpsilon);
            const double scale = denominator == 0.0 ? 0.0 : 1.0 / denominator;
            const double offset = (event.sweep_coordinate - site_z) / 2.0;
            auto [beach_iterator, inserted] = beach_surface.emplace(
                activated_site->index,
                voronoi::equisurface::Paraboloid(activated_site->site->position(), scale, offset));
            if (! inserted)
            {
                beach_iterator->second = voronoi::equisurface::Paraboloid(
                    activated_site->site->position(),
                    scale,
                    offset);
            }

            std::vector<std::size_t> candidate_neighbours;
            if (supported_vertices.size() <= 3)
            {
                candidate_neighbours.reserve(active_sites.size());
                for (const std::size_t neighbour_index : active_sites)
                {
                    if (neighbour_index != activated_site->index)
                    {
                        candidate_neighbours.push_back(neighbour_index);
                    }
                }
            }
            else
            {
                double best_height = std::numeric_limits<double>::infinity();
                for (const std::size_t neighbour_index : active_sites)
                {
                    if (neighbour_index == activated_site->index)
                    {
                        continue;
                    }

                    const auto beach_iterator_for_neighbour = beach_surface.find(neighbour_index);
                    if (beach_iterator_for_neighbour == beach_surface.end())
                    {
                        continue;
                    }

                    const double neighbour_height = beachHeightAt(
                        beach_iterator_for_neighbour->second,
                        activated_site->site->position());

                    if (neighbour_height + kContainmentEpsilon < best_height)
                    {
                        best_height = neighbour_height;
                        candidate_neighbours = { neighbour_index };
                    }
                    else if (std::abs(neighbour_height - best_height) <= kContainmentEpsilon)
                    {
                        candidate_neighbours.push_back(neighbour_index);
                    }
                }
            }

            for (const std::size_t neighbour_index : candidate_neighbours)
            {
                PatchPair* patch_pair = ensure_patch_pair(activated_site->index, neighbour_index);
                if (!patch_pair)
                {
                    continue;
                }

                patch_pair->first->addConstraint(makeSiteConstraint(activated_site->index, event.sweep_coordinate));
                patch_pair->second->addConstraint(makeSiteConstraint(activated_site->index, event.sweep_coordinate));
                patch_pair->first->addConstraint(makeBeachConstraint(*activated_site->site, event.sweep_coordinate, beach_iterator->second));
                patch_pair->second->addConstraint(makeBeachConstraint(*activated_site->site, event.sweep_coordinate, beach_iterator->second));
            }

            continue;
        }

        bool all_sites_active = std::ranges::all_of(
            event.site_indices,
            [&active_sites](const std::size_t site_index)
            {
                return active_sites.contains(site_index);
            });
        if (! all_sites_active || ! event.sphere_center)
        {
            continue;
        }

        for (std::size_t first_offset = 0; first_offset + 1 < event.site_indices.size(); ++first_offset)
        {
            for (std::size_t second_offset = first_offset + 1; second_offset < event.site_indices.size(); ++second_offset)
            {
                PatchPair* patch_pair = ensure_patch_pair(event.site_indices[first_offset], event.site_indices[second_offset]);
                if (! patch_pair)
                {
                    continue;
                }

                const std::string constraint = makeVertexConstraint(
                    *event.sphere_center,
                    event.sphere_radius,
                    event.sweep_coordinate,
                    event.site_indices);
                patch_pair->first->addConstraint(constraint);
                patch_pair->second->addConstraint(constraint);
            }
        }

        for (std::size_t pivot_offset = 0; pivot_offset < event.site_indices.size(); ++pivot_offset)
        {
            for (std::size_t first_offset = 0; first_offset < event.site_indices.size(); ++first_offset)
            {
                if (first_offset == pivot_offset)
                {
                    continue;
                }

                for (std::size_t second_offset = first_offset + 1; second_offset < event.site_indices.size(); ++second_offset)
                {
                    if (second_offset == pivot_offset)
                    {
                        continue;
                    }

                    PatchPair* first_pair = ensure_patch_pair(event.site_indices[pivot_offset], event.site_indices[first_offset]);
                    PatchPair* second_pair = ensure_patch_pair(event.site_indices[pivot_offset], event.site_indices[second_offset]);
                    if (! first_pair || ! second_pair)
                    {
                        continue;
                    }

                    auto first_patch = patch_for_site(
                        event.site_indices[pivot_offset],
                        event.site_indices[first_offset],
                        event.site_indices[pivot_offset]);
                    auto second_patch = patch_for_site(
                        event.site_indices[pivot_offset],
                        event.site_indices[second_offset],
                        event.site_indices[pivot_offset]);
                    if (! first_patch || ! second_patch)
                    {
                        continue;
                    }

                    first_patch->addAdjacentPatch(second_patch);
                    second_patch->addAdjacentPatch(first_patch);
                }
            }
        }
    }

    if (patch_pairs.empty())
    {
        spdlog::warn("The sweep-plane pass did not produce any finite Fortune events; falling back to pairwise point bisectors");

        for (std::size_t first_index = 0; first_index + 1 < supported_vertices.size(); ++first_index)
        {
            for (std::size_t second_index = first_index + 1; second_index < supported_vertices.size(); ++second_index)
            {
                PatchPair* patch_pair = ensure_patch_pair(supported_vertices[first_index].index, supported_vertices[second_index].index);
                if (! patch_pair)
                {
                    continue;
                }

                patch_pair->first->addConstraint("fortune.degenerate pairwise fallback");
                patch_pair->second->addConstraint("fortune.degenerate pairwise fallback");
            }
        }
    }

    return output_diagram;
}

std::shared_ptr<voronoi::equisurface::AbstractBisector> Generator::generateBisector(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    for (const auto& bisector_generator : bisector_generators_)
    {
        if (! bisector_generator)
        {
            continue;
        }

        const auto generated_bisector = bisector_generator->generate(first_site, second_site);
        if (generated_bisector)
        {
            return generated_bisector;
        }
    }

    return nullptr;
}

} // namespace yav::generator
