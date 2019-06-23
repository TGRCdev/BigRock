#ifndef BIGROCK_ELLIPSOID_H
#define BIGROCK_ELLIPSOID_H
#pragma once

#include "../tool.hpp"

namespace bigrock {
namespace data {
namespace tools {

struct Ellipsoid : public Tool
{
    float value_local(glm::vec3 point) const;
    inline ToolType get_tool_type() const {return TOOL_TYPE_ELLIPSOID;}
    bool is_concave() const {return false;}
    int get_max_depth() const {return -1;}
};

}}}

#endif