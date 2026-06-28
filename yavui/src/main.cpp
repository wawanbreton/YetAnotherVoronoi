// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <QApplication>
#include <QLabel>
#include <cxxopts.hpp>
#include <iostream>
#include <ranges>
#include <rapidobj/rapidobj.hpp>
#include <set>
#include <string>
#include <yav/generator/Generator.h>
#include <yav/generator/VoronoiQuadtreeNode.h>
#include <yav/space/Space2.h>
#include <yav/voronoi/Diagram.h>

#include <fmt/chrono.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include "yavui/VoronoiGraphicsView.h"


int main(int argc, char** argv)
{
    cxxopts::Options options("yav", "Minimal CLI for libyav");
    // clang-format off
    options.add_options()
        ("h,help", "Print help")
        ("d,debug", "Display debug-level output")
        ("l,leaves", "Display the generated quad/octree")
        ("t,tree", "Display the leaves of the tree")
        ("n,no-ui", "Do not display the UI, stop after execution")
        ("file", "Path of the mesh file to be loaded", cxxopts::value<std::string>())
        ("random-points", "Number of random points to be generated", cxxopts::value<size_t>())
        ("random-edges", "Number of random edges to be generated", cxxopts::value<size_t>());
    // clang-format on
    options.parse_positional({ "file" });
    options.positional_help("<file>");
    options.show_positional_help();

    const cxxopts::ParseResult options_result = options.parse(argc, argv);

    if (options_result.count("help") > 0U
        || (options_result.count("file") == 0 && options_result.count("random-points") == 0 && options_result.count("random-edges") == 0))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (options_result.count("debug") > 0U)
    {
        spdlog::set_level(spdlog::level::debug);
    }

    yav::Space2 space;

    if (options_result.count("file") != 0)
    {
        rapidobj::Result result = rapidobj::ParseFile(options_result["file"].as<std::string>());
        if (result.error)
        {
            spdlog::error("Error when loading mesh: {}", result.error.code.message());
            return 1;
        }

        std::set<int> used_vertices;
        std::set<std::pair<int, int>> used_edges;
        const rapidobj::Array<float>& positions = result.attributes.positions;

        // Loose vertices
        for (const auto& [index, position] : result.attributes.positions | std::views::chunk(3) | std::views::enumerate)
        {
            if (! std::ranges::contains(used_vertices, index) && position.size() == 3)
            {
                space.addVertex(yav::Point2(position[0], position[1]));
            }
        }
    }

    if (options_result.count("random-points") != 0)
    {
        auto points = options_result["random-points"].as<size_t>();
        for (size_t i = 0; i < points; ++i)
        {
            space.addVertex(yav::Point2(static_cast<double>(std::rand()) / RAND_MAX, static_cast<double>(std::rand()) / RAND_MAX));
        }
    }

    if (options_result.count("random-edges") != 0)
    {
        auto edges = options_result["random-edges"].as<size_t>();
        for (size_t i = 0; i < edges; ++i)
        {
            space.addEdge(yav::Segment2(
                yav::Point2(static_cast<double>(std::rand()) / RAND_MAX, static_cast<double>(std::rand()) / RAND_MAX),
                yav::Point2(static_cast<double>(std::rand()) / RAND_MAX, static_cast<double>(std::rand()) / RAND_MAX)));
        }
    }

    space.calculateAutoBoundingBox(1.2);
    spdlog::info("Using bounding box {}", space.boundingBox());

    yav::Generator generator(6);

    spdlog::info("Generate diagram with {} sites", space.sites().size());
    spdlog::stopwatch timer;
    auto [voronoi_diagram, tree, leaves] = generator.generate(space);
    spdlog::info("Generated {} cells in {}", voronoi_diagram->cells().size(), timer.elapsed());

    if (options_result.count("no-ui") != 0)
    {
        return EXIT_SUCCESS;
    }

    QApplication app(argc, argv);

    VoronoiGraphicsView graphics_view;
    graphics_view.setSpace(space);
    graphics_view.setDiagram(*voronoi_diagram);

    if (options_result.count("tree"))
    {
        graphics_view.setTree(tree);
    }

    if (options_result.count("leaves"))
    {
        graphics_view.setTreeLeaves(leaves);
    }

    graphics_view.resize(800, 600);
    graphics_view.autoFit();
    graphics_view.show();

    QObject::connect(
        &graphics_view,
        &VoronoiGraphicsView::approximateNode,
        &graphics_view,
        [&space](const yav::VoronoiQuadtreeNode::Ptr& node)
        {
            yav::Diagram local_diagram; // Avoid modifying the originally generated diagram
            yav::Generator::addApproximationFromLeaf(*node, local_diagram, space);
        });

    return app.exec();
}
