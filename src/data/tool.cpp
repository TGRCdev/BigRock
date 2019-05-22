#include "tool.hpp"
#include "../grid_vertices.hpp"
#include "serialize.hpp"

// Tools
#include "tools/ellipsoid.hpp"

namespace bigrock {
namespace data {

float Tool::value(glm::vec3 point) const
{
    return value_local(transform.to_local(point));
}

Tool *Tool::new_tool(char tooltype)
{
    switch(tooltype)
    {
        case TOOL_TYPE_ELLIPSOID:
            return new tools::Ellipsoid();
        default:
            return NULL;
    }
}

AABB Tool::get_aabb() const
{
    AABB ret;
    for(int i = 0; i < 8; i++)
        ret.extend(GRID_VERTICES[i] - glm::vec3(0.5f));
    return ret;
}

Tool *Tool::deserialize_tool(const char *buffer, int *chars_read)
{
    if(chars_read)
        *chars_read = 0;

    if(!buffer)
        return NULL;

    unsigned int pos = 0;
    if(buffer[0] == 'B' && buffer[1] == 'R')
        pos = 2;
    
    if(buffer[pos++] != 'T')
        return NULL;

    Tool *t;

    switch(buffer[pos++])
    {
        case TOOL_TYPE_ELLIPSOID:
        {
            t = new Ellipsoid();
            pos += deserialize<Ellipsoid>(*static_cast<Ellipsoid*>(t), buffer+pos);
            break;
        }
        default:
        {
            t = NULL;
            pos = 0;
            break;
        }
    }

    if(chars_read)
        *chars_read = pos;
    return t;
}

int Tool::serialize_tool(const Tool &t, char *buffer, bool include_prefix)
{
    if(!buffer)
        return 0;
    
    if(t.get_tool_type() == TOOL_TYPE_NULL)
        return 0;
    
    unsigned int chars = 0;
    if(include_prefix)
    {
        buffer[chars++] = 'B';
        buffer[chars++] = 'R';
    }
    buffer[chars++] = 'T';
    buffer[chars++] = t.get_tool_type();
    switch(t.get_tool_type())
    {
        case TOOL_TYPE_ELLIPSOID:
        {
            chars += serialize<Ellipsoid>(*reinterpret_cast<const Ellipsoid*>(&t), buffer + chars, false);
        }
    }
    return chars;
}

}}