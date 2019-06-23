#ifndef BIGROCK_ACTION_H
#define BIGROCK_ACTION_H
#pragma once

#include "query_structs.hpp"
#include "tool.hpp"
#include "point.hpp"

namespace bigrock {
namespace data {

// Derived Tool data
struct EmplaceData
{
    unsigned char material;
};

struct Action
{
    enum ActionType
    {
        ACTION_TYPE_EMPLACE
    };

    union{ // Tagged by get_action_type()
        EmplaceData emplace;
    } data;

    /// Updates the Point using the tool and this action.
    virtual void update(const Tool &t, const glm::vec3 position, Point &v, ToolQueryResult *res = NULL) const = 0;
    
    virtual ActionType get_action_type() const = 0;

    /// Returns a UTF-8 string with the encoded action.
    /// Returns an empty string on error.
    std::string serialize() const;

    /// Returns the Action stored in the buffer
    /// Returns nullptr if the buffer is invalid
    static std::unique_ptr<Action> deserialize(const void *buf, size_t length);
};

}}

#endif