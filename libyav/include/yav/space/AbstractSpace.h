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

namespace primitive
{
class AbstractPrimitive;
}

/** Top-level user input container for Voronoi diagram generation. */
class AbstractSpace
{
public:
    AbstractSpace();

    const std::vector<std::shared_ptr<primitive::AbstractPrimitive>>& primitives() const;

protected:
    std::shared_ptr<primitive::AbstractPrimitive> addPrimitive(const std::shared_ptr<primitive::AbstractPrimitive>& primitive);

private:
    std::vector<std::shared_ptr<primitive::AbstractPrimitive>> primitives_;
};

} // namespace yav::space
