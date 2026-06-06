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

namespace site
{
class AbstractSite;
}

/** Top-level user input container for Voronoi diagram generation. */
class AbstractSpace
{
public:
    AbstractSpace();

    const std::vector<std::shared_ptr<site::AbstractSite>>& primitives() const;

protected:
    std::shared_ptr<site::AbstractSite> addSite(const std::shared_ptr<site::AbstractSite>& site);

private:
    std::vector<std::shared_ptr<site::AbstractSite>> sites_;
};

} // namespace yav::space
