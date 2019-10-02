#ifndef BIGROCK_EMPLACE_ACTION_H
#define BIGROCK_EMPLACE_ACTION_H
#pragma once

#include "../action.hpp"

namespace bigrock {
    /// Emplace Action
    ///
    /// Updates points by placing the specified terrain details on any
    /// points that fall within the Shape given.
    struct Emplace : public Action
    {
        material_t material;

        void update(const Shape &shape, glm::vec3 position, Point &point) const;
    };
}

#endif