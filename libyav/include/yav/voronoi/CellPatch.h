// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/voronoi/equisurface/AbstractBisector.h"

#include <memory>
#include <string>
#include <vector>

namespace yav::voronoi
{

/** Portion of a cell boundary attached to one bisector and optional constraints. */
class CellPatch : public std::enable_shared_from_this<CellPatch>
{
public:
    CellPatch();

    void setBisector(const std::shared_ptr<equisurface::AbstractBisector>& bisector);
    std::shared_ptr<equisurface::AbstractBisector> bisector() const;

    void addConstraint(const std::string& constraint_expression);
    const std::vector<std::string>& constraints() const;

    void addAdjacentPatch(const std::shared_ptr<CellPatch>& patch);
    const std::vector<std::weak_ptr<CellPatch>>& adjacentPatches() const;

private:
    std::shared_ptr<equisurface::AbstractBisector> bisector_;
    std::vector<std::string> constraints_;
    std::vector<std::weak_ptr<CellPatch>> adjacent_patches_;
};

} // namespace yav::voronoi
