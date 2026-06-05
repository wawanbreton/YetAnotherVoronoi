// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/AbstractSpace.h"

#include <algorithm>

#include <spdlog/spdlog.h>

#include "yav/space/Primitive.h"
#include "yav/space/site/AbstractSite.h"


namespace yav::space
{

AbstractSpace::AbstractSpace() = default;

const std::vector<std::shared_ptr<Primitive>>& AbstractSpace::primitives() const
{
    return primitives_;
}

std::shared_ptr<Primitive> AbstractSpace::addPrimitive(const std::vector<yav::space::site::AbstractSite::Ptr>& sites)
{
    return primitives_.emplace_back(std::make_shared<Primitive>(sites));
}

} // namespace yav::space
