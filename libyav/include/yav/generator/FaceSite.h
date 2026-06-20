// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <memory>

#include "yav/geometry/Segment2.h"

namespace yav
{

class AbstractSite;

struct FaceSite
{
    std::shared_ptr<AbstractSite> site;
    Segment2 face;
    Segment2 closest_segment_part;
};

} // namespace yav
