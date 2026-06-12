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
#include <yav/space/Space2.h>
#include <yav/voronoi/Diagram.h>

#include <spdlog/spdlog.h>

#include "yavui/VoronoiGraphicsView.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // QLabel label(QString::fromStdString("yavui using " + yav::version()));
    // label.resize(320, 80);
    // label.show();

    cxxopts::Options options("yav", "Minimal CLI for libyav");
    options.add_options()("h,help", "Print help")("d,debug", "Display debug-level output")(
        "file",
        "Path of the mesh file to be loaded",
        cxxopts::value<std::string>())("random", "Number of random points to be generated", cxxopts::value<size_t>());
    options.parse_positional({ "file" });
    options.positional_help("<file>");

    const cxxopts::ParseResult options_result = options.parse(argc, argv);

    if (options_result.count("help") > 0U || (options_result.count("file") == 0 && options_result.count("random") == 0))
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

    if (options_result.count("random") != 0)
    {
        auto points = options_result["random"].as<size_t>();
        for (size_t i = 0; i < points; ++i)
        {
            space.addVertex(yav::Point2(static_cast<double>(std::rand()) / RAND_MAX, static_cast<double>(std::rand()) / RAND_MAX));
        }
    }

    yav::Generator generator;
    const yav::Diagram voronoi_diagram = generator.generate(space);
    spdlog::info("Generated {} cells", voronoi_diagram.cells().size());

    VoronoiGraphicsView graphics_view;
    graphics_view.setSpace(space);
    graphics_view.setDiagram(voronoi_diagram);
    graphics_view.resize(800, 600);
    graphics_view.autoFit();
    graphics_view.show();

    return app.exec();
}
