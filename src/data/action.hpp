#ifndef BIGROCK_ACTION_H
#define BIGROCK_ACTION_H
#pragma once

#include "query_structs.hpp"
#include "tool.hpp"
#include "point.hpp"

namespace bigrock {
namespace data {

struct Action
{
    /// Updates the Point using the tool and this action.
    virtual void update(const Tool &t, Point &v, ToolQueryResult *res = NULL) = 0;
};

}}

#endif