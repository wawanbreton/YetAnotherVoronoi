// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge2.h"


namespace yav::space::site
{

Edge2::Edge2(const geometry::Segment2& segment)
    : segment_(segment)
{
}

const geometry::Segment2& Edge2::segment() const
{
    return segment_;
}

} // namespace yav::space::site
