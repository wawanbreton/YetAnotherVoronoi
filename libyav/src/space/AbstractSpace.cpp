// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/AbstractSpace.h"


namespace yav::space
{

AbstractSpace::AbstractSpace() = default;

const std::vector<std::shared_ptr<primitive::AbstractPrimitive>>& AbstractSpace::primitives() const
{
    return primitives_;
}

std::shared_ptr<primitive::AbstractPrimitive> AbstractSpace::addPrimitive(const std::shared_ptr<primitive::AbstractPrimitive>& primitive)
{
    primitives_.push_back(primitive);
    return primitive;
}

} // namespace yav::space
