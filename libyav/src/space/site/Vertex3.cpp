// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Vertex3.h"


namespace yav::space::site
{

Vertex3::Vertex3(const geometry::Point3& position)
    : position_(position)
{
}

const geometry::Point3& Vertex3::position() const
{
    return position_;
}

} // namespace yav::space::site
