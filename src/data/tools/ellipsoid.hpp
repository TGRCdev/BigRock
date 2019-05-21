#ifndef BIGROCK_ELLIPSOID_H
#define BIGROCK_ELLIPSOID_H
#pragma once

#include "../tool.hpp"

namespace bigrock {
namespace data {
namespace tools {

struct Ellipsoid : public Tool
{
    private:
    static std::list<Member<Ellipsoid> > members;
    public:

    float value_local(glm::vec3 point) const;
    inline ToolType get_tool_type() const {return TOOL_TYPE_ELLIPSOID;}

    template<class T>
    friend std::list<Member<T> > bigrock::data::get_member_list(); // This pisses me off but I can't fix it
};

}

using namespace tools;

template<>
std::list<Member<Ellipsoid> > get_member_list();
}}

#endif