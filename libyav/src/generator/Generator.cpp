// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include "yav/generator/bisector/EdgeEdgeBisectorGenerator.h"
#include "yav/generator/bisector/EdgeVertexBisectorGenerator.h"
#include "yav/generator/bisector/FaceEdgeBisectorGenerator.h"
#include "yav/generator/bisector/FaceFaceBisectorGenerator.h"
#include "yav/generator/bisector/FaceVertexBisectorGenerator.h"
#include "yav/generator/bisector/VertexVertexBisectorGenerator.h"
#include "yav/generator/region/Cone.h"
#include "yav/generator/region/HalfPlaneSlice.h"
#include "yav/generator/region/Slab.h"
#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"

#include <boost/geometry/core/access.hpp>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <ranges>

namespace yav::generator
{
namespace
{

geometry::Point3 makePoint(const double x, const double y, const double z)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, x);
    boost::geometry::set<1>(output, y);
    boost::geometry::set<2>(output, z);
    return output;
}

double averageZ(const std::array<geometry::Point3, 3>& vertices)
{
    return (boost::geometry::get<2>(vertices[0]) + boost::geometry::get<2>(vertices[1])
               + boost::geometry::get<2>(vertices[2]))
        / 3.0;
}

} // namespace

Generator::Generator()
    : bisector_generators_(
          { std::make_shared<bisector::FaceFaceBisectorGenerator>(),
            std::make_shared<bisector::FaceEdgeBisectorGenerator>(),
            std::make_shared<bisector::FaceVertexBisectorGenerator>(),
            std::make_shared<bisector::EdgeEdgeBisectorGenerator>(),
            std::make_shared<bisector::EdgeVertexBisectorGenerator>(),
            std::make_shared<bisector::VertexVertexBisectorGenerator>() })
{
}

voronoi::Diagram Generator::generate(const space::Space& input_space) const
{
    voronoi::Diagram output_diagram;

    std::ranges::for_each(
        input_space.primitives(),
        [&output_diagram](const std::shared_ptr<space::Primitive>& primitive)
        {
            if (!primitive)
            {
                spdlog::error("Encountered null primitive while initializing Voronoi cells");
                return;
            }

            output_diagram.addCell(std::make_shared<voronoi::Cell>(primitive->primitiveId()));
        });

    std::size_t patch_id = 0;

    const auto& primitives = input_space.primitives();
    for (std::size_t first_primitive_index = 0; first_primitive_index < primitives.size(); ++first_primitive_index)
    {
        for (std::size_t second_primitive_index = first_primitive_index + 1; second_primitive_index < primitives.size();
             ++second_primitive_index)
        {
            const auto& first_primitive = primitives[first_primitive_index];
            const auto& second_primitive = primitives[second_primitive_index];

            if (!first_primitive || !second_primitive)
            {
                spdlog::error("Encountered null primitive while generating bisector patches");
                continue;
            }

            for (const auto& first_site : first_primitive->sites())
            {
                for (const auto& second_site : second_primitive->sites())
                {
                    if (!first_site || !second_site)
                    {
                        spdlog::error("Encountered null site while generating bisector patches");
                        continue;
                    }

                    const auto bisector_generator =
                        findBisectorGenerator(first_site->siteKind(), second_site->siteKind());

                    if (!bisector_generator)
                    {
                        spdlog::warn(
                            "No bisector generator registered for site pair {} / {}",
                            static_cast<int>(first_site->siteKind()),
                            static_cast<int>(second_site->siteKind()));
                        continue;
                    }

                    const auto generated_bisector = bisector_generator->generate(first_site, second_site);
                    if (!generated_bisector)
                    {
                        continue;
                    }

                    const auto first_cell = output_diagram.findCell(first_primitive->primitiveId());
                    const auto second_cell = output_diagram.findCell(second_primitive->primitiveId());
                    if (!first_cell || !second_cell)
                    {
                        spdlog::error("Cannot attach generated patch because one of the target cells is missing");
                        continue;
                    }

                    const auto first_patch = std::make_shared<voronoi::CellPatch>(patch_id++);
                    first_patch->setBisector(generated_bisector);
                    // TODO: formal constraints model is not defined yet, so this remains a placeholder.
                    first_patch->addConstraint("placeholder: pending formal clipping constraints model");

                    const auto second_patch = std::make_shared<voronoi::CellPatch>(patch_id++);
                    second_patch->setBisector(generated_bisector);
                    second_patch->addConstraint("placeholder: pending formal clipping constraints model");

                    first_patch->addAdjacentPatch(second_patch);
                    second_patch->addAdjacentPatch(first_patch);

                    first_cell->addPatch(first_patch);
                    second_cell->addPatch(second_patch);
                }
            }
        }
    }

    std::ranges::for_each(
        primitives,
        [this](const std::shared_ptr<space::Primitive>& primitive)
        {
            if (!primitive)
            {
                return;
            }

            std::vector<std::shared_ptr<region::AbstractVoronoiRegion>> generated_regions;
            generated_regions.reserve(primitive->sites().size());

            std::ranges::transform(
                primitive->sites(),
                std::back_inserter(generated_regions),
                [this](const std::shared_ptr<space::site::AbstractSite>& current_site)
                {
                    return buildRegionForSite(current_site);
                });

            const std::size_t valid_region_count =
                std::ranges::count_if(generated_regions, [](const auto& region) { return static_cast<bool>(region); });

            spdlog::debug(
                "Generated {} local regions for primitive {}",
                valid_region_count,
                primitive->primitiveId());
        });

    return output_diagram;
}

std::shared_ptr<bisector::AbstractBisectorGenerator> Generator::findBisectorGenerator(
    const space::site::SiteKind first_kind,
    const space::site::SiteKind second_kind) const
{
    const auto matching_generator_iterator = std::ranges::find_if(
        bisector_generators_,
        [first_kind, second_kind](const std::shared_ptr<bisector::AbstractBisectorGenerator>& bisector_generator)
        {
            return bisector_generator && bisector_generator->canHandle(first_kind, second_kind);
        });

    if (matching_generator_iterator == bisector_generators_.end())
    {
        return nullptr;
    }

    return *matching_generator_iterator;
}

std::shared_ptr<region::AbstractVoronoiRegion> Generator::buildRegionForSite(
    const std::shared_ptr<space::site::AbstractSite>& site) const
{
    if (!site)
    {
        spdlog::error("Cannot generate region from a null site");
        return nullptr;
    }

    if (site->siteKind() == space::site::SiteKind::Triangle)
    {
        const auto triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(site);
        if (!triangle_site)
        {
            spdlog::error("Triangle site kind was declared but cast failed");
            return nullptr;
        }

        const double mean_z = averageZ(triangle_site->vertices());
        return std::make_shared<region::Slab>(makePoint(0.0, 0.0, 1.0), mean_z - 1.0, mean_z + 1.0);
    }

    if (site->siteKind() == space::site::SiteKind::Edge)
    {
        const auto edge_site = std::dynamic_pointer_cast<space::site::Edge>(site);
        if (!edge_site)
        {
            spdlog::error("Edge site kind was declared but cast failed");
            return nullptr;
        }

        const double mean_z =
            (boost::geometry::get<2>(edge_site->vertices()[0]) + boost::geometry::get<2>(edge_site->vertices()[1])) / 2.0;
        return std::make_shared<region::HalfPlaneSlice>(makePoint(0.0, 0.0, 1.0), -mean_z);
    }

    if (site->siteKind() == space::site::SiteKind::Vertex)
    {
        const auto vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(site);
        if (!vertex_site)
        {
            spdlog::error("Vertex site kind was declared but cast failed");
            return nullptr;
        }

        return std::make_shared<region::Cone>(vertex_site->position(), makePoint(0.0, 0.0, 1.0), 0.75);
    }

    spdlog::error("Unsupported site kind in region generation");
    return nullptr;
}

} // namespace yav::generator
