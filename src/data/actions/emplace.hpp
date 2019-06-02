#ifndef BIGROCK_EMPLACE_H
#define BIGROCK_EMPLACE_H
#pragma once

#include "../action.hpp"

namespace bigrock {
namespace data {
namespace actions {

struct Emplace : public Action
{
    Emplace(const unsigned char material = 0);

    ActionType get_action_type() const {return ACTION_TYPE_EMPLACE;}
    void update(const Tool &t, const Vector3 position, Point &v, ToolQueryResult *res) const;
};

}}}

#endif