// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <fmt/format.h>

#include "yav/geometry/Point2.h"


namespace yav
{

using Box2 = boost::geometry::model::box<Point2>;

} // namespace yav


template<>
struct fmt::formatter<yav::Box2> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::Box2& box, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(min{},max{})", box.min_corner(), box.max_corner());
    }
};
