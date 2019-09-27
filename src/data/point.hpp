#ifndef BIGROCK_POINT_H
#define BIGROCK_POINT_H
#pragma once

#include "../defines.hpp"

namespace bigrock {

// Position of Point can be retrieved from the containing Cell
struct Point
{
    isovalue_t density;
    material_t material;

    static constexpr isovalue_t MIN_DENSITY = -1.0f;
    static constexpr isovalue_t MAX_DENSITY = 1.0f;

    static constexpr material_t MIN_MATERIAL = 0;
    static constexpr material_t MAX_MATERIAL = 0xFF;

    Point(material_t material = MIN_MATERIAL, isovalue_t density = MIN_DENSITY) : 
    material(material), density(density) {}

    /// Interpolate linearly along two points,
    /// with `t` as the percentage from `this` to `other`
    Point lerp(const Point other, float t) const;

    /// Returns true if the two points are identical, and can
    /// be collapsed into one point.
    ///
    /// Collapsability is reflexive. Point A will always be 
    /// collapsible into itself.
    ///
    /// Collapsability is symmetric. If Point A can collapse
    /// into Point B, then B can collapse into A as well.
    ///
    /// Collapsability is transitive. If Point A can collapse
    /// with Point B, and B can collapse with Point C, then A can
    /// collapse with C.
    bool can_collapse(const Point other) const;
};

}

#endif