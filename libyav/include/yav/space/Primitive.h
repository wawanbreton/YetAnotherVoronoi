// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/space/site/AbstractSite.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace yav::space
{

/** Input primitive grouping one logical object and all associated sites. */
class Primitive
{
public:
    explicit Primitive(std::size_t primitive_id);

    std::size_t primitiveId() const;
    void addSite(const std::shared_ptr<site::AbstractSite>& site);
    const std::vector<std::shared_ptr<site::AbstractSite>>& sites() const;

private:
    std::size_t primitive_id_;
    std::vector<std::shared_ptr<site::AbstractSite>> sites_;
};

} // namespace yav::space
