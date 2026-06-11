// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <memory>

namespace yav
{

class AbstractSite;

struct ClosestSite
{
    std::shared_ptr<AbstractSite> site;
    double distance;
};

} // namespace yav
