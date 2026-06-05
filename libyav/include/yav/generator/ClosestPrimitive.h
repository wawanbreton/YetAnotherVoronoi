// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <memory>

namespace yav
{

namespace space
{
class Primitive;
}

namespace generator
{

struct ClosestPrimitive
{
    std::shared_ptr<space::Primitive> primitive;
    double distance;
};

} // namespace generator

} // namespace yav
