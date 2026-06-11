// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav
{

class AbstractSite;

/** Top-level user input container for Voronoi diagram generation. */
class AbstractSpace
{
public:
    AbstractSpace();

    const std::vector<std::shared_ptr<AbstractSite>>& sites() const;

protected:
    std::shared_ptr<AbstractSite> addSite(const std::shared_ptr<AbstractSite>& site);

private:
    std::vector<std::shared_ptr<AbstractSite>> sites_;
};

} // namespace yav
