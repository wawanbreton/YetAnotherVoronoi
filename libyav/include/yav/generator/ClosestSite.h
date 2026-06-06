// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <memory>

namespace yav
{

namespace space::site
{
class AbstractSite;
}

namespace generator
{

struct ClosestSite
{
    std::shared_ptr<space::site::AbstractSite> site;
    double distance;
};

} // namespace generator

} // namespace yav
