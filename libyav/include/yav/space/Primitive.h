// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav::space
{

namespace site
{
class AbstractSite;
}

/** Input primitive grouping one logical object and all associated sites. */
class Primitive
{
public:
    Primitive();

    void addSite(const std::shared_ptr<site::AbstractSite>& site);
    const std::vector<std::shared_ptr<site::AbstractSite>>& sites() const;

private:
    std::vector<std::shared_ptr<site::AbstractSite>> sites_;
};

} // namespace yav::space
