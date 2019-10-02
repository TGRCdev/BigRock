#ifndef BIGROCK_ACTION_H
#define BIGROCK_ACTION_H
#pragma once

#include "shape.hpp"
#include "point.hpp"

namespace bigrock {
    // Abstract class for Point editing actions
    struct Action
    {
        /// Updates the given Point using the given Shape and whatever
        /// this Action is defined to do.
        virtual void update(const Shape &shape, glm::vec3 position, Point &point) const = 0;

        // TODO: Serialization, Deserialization
    };
}

#endif