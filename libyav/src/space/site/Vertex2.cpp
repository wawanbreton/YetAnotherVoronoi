// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Vertex2.h"


namespace yav::space::site
{

Vertex2::Vertex2(const geometry::Point2& position)
    : position_(position)
{
}

const geometry::Point2& Vertex2::position() const
{
    return position_;
}

} // namespace yav::space::site
