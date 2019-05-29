#include "tool.hpp"
#include "../grid_vertices.hpp"
#include "schemas/tool_generated.h"

// Tools
#include "tools/ellipsoid.hpp"

namespace bigrock {
namespace data {

float Tool::value(glm::vec3 point) const
{
    return value_local(transform.to_local(point));
}

AABB Tool::get_aabb() const
{
    AABB ret;
    for(int i = 0; i < 8; i++)
        ret.extend(GRID_VERTICES[i] - glm::vec3(0.5f));
    return ret;
}

std::string Tool::serialize() const
{
    schemas::ToolType ttype;
    switch(get_tool_type())
    {
        case TOOL_TYPE_ELLIPSOID:
        ttype = schemas::ToolType_Ellipsoid;
        break;
        default:
        return std::string();
    }
    flatbuffers::FlatBufferBuilder builder;
    schemas::Transform trns = transform;
    auto tool = schemas::CreateTool(builder, &trns, ttype);
    builder.Finish(tool);
    return std::string(reinterpret_cast<char*>(builder.GetBufferPointer()), static_cast<size_t>(builder.GetSize()));
}

std::unique_ptr<Tool> Tool::deserialize(const void *buf, size_t length)
{
    if(buf == NULL)
        return nullptr;
    flatbuffers::Verifier v = flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(buf), length);
    bool ok = schemas::VerifyToolBuffer(v);
    if(!ok)
        return nullptr;
    
    auto tool = schemas::GetTool(buf);
    Tool *newtool = NULL;
    switch(tool->tool_type())
    {
        case schemas::ToolType_Ellipsoid:
        newtool = new tools::Ellipsoid();
        break;
    }

    if(!newtool)
        return nullptr;
    
    newtool->transform = tool->transform();
    return std::unique_ptr<Tool>(newtool);
}

}}