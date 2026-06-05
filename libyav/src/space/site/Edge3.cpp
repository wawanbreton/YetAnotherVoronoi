// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge3.h"


namespace yav::space::site
{

Edge3::Edge3(const geometry::Segment3& segment)
    : segment_(segment)
{
}

const geometry::Segment3& Edge3::segment() const
{
    return segment_;
}

} // namespace yav::space::site
