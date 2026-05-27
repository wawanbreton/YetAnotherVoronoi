// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <cxxopts.hpp>
#include <iostream>

#include "yav/yav.hpp"

int main(int argc, char** argv)
{
    cxxopts::Options options("yav", "Minimal CLI for libyav");
    options.add_options()("v,version", "Print libyav version")("h,help", "Print help");

    const auto result = options.parse(argc, argv);

    if (result.count("help") > 0U)
    {
        std::cout << options.help() << '\n';
        return 0;
    }

    if (result.count("version") > 0U)
    {
        std::cout << yav::version() << '\n';
        return 0;
    }

    std::cout << "Run with --help to list options.\n";
    return 0;
}
