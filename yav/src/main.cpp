// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <ranges>
#include <rapidobj/rapidobj.hpp>
#include <set>

#include <spdlog/spdlog.h>

#include "yav/generator/Generator.h"
#include "yav/geometry/Point3.h"
#include "yav/geometry/Segment3.h"
#include "yav/space/Space2.h"
#include "yav/voronoi/Cell.h"
#include "yav/voronoi/Diagram.h"
#include "yav/yav.hpp"


int main(int argc, char** argv)
{
    cxxopts::Options options("yav", "Minimal CLI for libyav");
    options.add_options()("v,version", "Print libyav version")("h,help", "Print help")(
        "d,debug",
        "Display debug-level output")("file", "Path of the mesh file to be loaded", cxxopts::value<std::string>());
    options.parse_positional({ "file" });
    options.positional_help("<file>");

    const cxxopts::ParseResult options_result = options.parse(argc, argv);

    if (options_result.count("help") > 0U || options_result.count("file") == 0)
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (options_result.count("version") > 0U)
    {
        std::cout << yav::version() << std::endl;
        return 0;
    }

    if (options_result.count("debug") > 0U)
    {
        spdlog::set_level(spdlog::level::debug);
    }

    rapidobj::Result result = rapidobj::ParseFile(options_result["file"].as<std::string>());
    if (result.error)
    {
        spdlog::error("Error when loading mesh: {}", result.error.code.message());
        return 1;
    }

    yav::space::Space2 space;
    std::set<int> used_vertices;
    std::set<std::pair<int, int>> used_edges;
    const rapidobj::Array<float>& positions = result.attributes.positions;

    const auto point_at_index = [&positions](const rapidobj::Index& index) -> yav::geometry::Point3
    {
        return yav::geometry::Point3(
            positions[index.position_index * 3],
            positions[index.position_index * 3 + 1],
            positions[index.position_index * 3 + 2]);
    };

    // Triangles
    // for (const rapidobj::Shape& shape : result.shapes)
    // {
    //     size_t offset = 0;
    //     for (const uint8_t face_num_vertices : shape.mesh.num_face_vertices)
    //     {
    //         if (face_num_vertices == 3)
    //         {
    //             const rapidobj::Index& idx1 = shape.mesh.indices[offset + 0];
    //             const rapidobj::Index& idx2 = shape.mesh.indices[offset + 1];
    //             const rapidobj::Index& idx3 = shape.mesh.indices[offset + 2];

    //             space.addFace({ point_at_index(idx1), point_at_index(idx2), point_at_index(idx3) });

    //             used_vertices.insert(idx1.position_index);
    //             used_vertices.insert(idx2.position_index);
    //             used_vertices.insert(idx3.position_index);
    //             used_edges.insert({ idx1.position_index, idx2.position_index });
    //             used_edges.insert({ idx2.position_index, idx3.position_index });
    //             used_edges.insert({ idx3.position_index, idx1.position_index });
    //             used_edges.insert({ idx2.position_index, idx1.position_index });
    //             used_edges.insert({ idx3.position_index, idx2.position_index });
    //             used_edges.insert({ idx1.position_index, idx3.position_index });
    //         }

    //         offset += face_num_vertices;
    //     }
    // }

    // Loose edges
    // for (const rapidobj::Shape& shape : result.shapes)
    // {
    //     size_t offset = 0;
    //     for (const uint8_t line_num_vertices : shape.lines.num_line_vertices)
    //     {
    //         for (uint8_t i = 0; i < line_num_vertices - 1; ++i)
    //         {
    //             const rapidobj::Index& idx1 = shape.lines.indices[offset + i];
    //             const rapidobj::Index& idx2 = shape.lines.indices[offset + i + 1];

    //             if (! std::ranges::contains(used_edges, std::make_pair(idx1.position_index, idx2.position_index)))
    //             {
    //                 space.addEdge({ point_at_index(idx1), point_at_index(idx2) });

    //                 used_vertices.insert(idx1.position_index);
    //                 used_vertices.insert(idx2.position_index);
    //             }
    //         }

    //         offset += line_num_vertices;
    //     }
    // }

    // Loose vertices
    for (const auto& [index, position] : result.attributes.positions | std::views::chunk(3) | std::views::enumerate)
    {
        if (! std::ranges::contains(used_vertices, index) && position.size() == 3)
        {
            space.addVertex(yav::geometry::Point2(position[0], position[1]));
        }
    }

    yav::generator::Generator generator;
    const yav::voronoi::Diagram voronoi_diagram = generator.generate(space);

    spdlog::info("Generated {} cells", voronoi_diagram.cells().size());

    return 0;
}
