#include "tool.hpp"
#include "../grid_vertices.hpp"
#include "schemas/tool_generated.h"

// Tools
#include "tools/ellipsoid.hpp"

#if BR_USE_DOUBLE_PRECISION
#define TransformType schemas::Transformd
#define TransformTag schemas::Transform_Transformd
#else
#define TransformType schemas::Transformf
#define TransformTag schemas::Transform_Transformf
#endif

namespace bigrock {
namespace data {

float Tool::value(Vector3 point) const
{
    return value_local(transform.to_local(point));
}

AABB Tool::get_aabb() const
{
    AABB ret;
    for(int i = 0; i < 8; i++)
        ret.extend(GRID_VERTICES[i] - Vector3(0.5f));
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
    auto trns = builder.CreateStruct<TransformType>(this->transform);
    auto tool = schemas::CreateTool(builder, TransformTag, trns.Union(), ttype);
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
    
    auto trnstype = tool->transform_type();
    switch(trnstype)
    {
        case schemas::Transform_Transformd:
        newtool->transform = tool->transform_as_Transformd();
        break;
        case schemas::Transform_Transformf:
        newtool->transform = tool->transform_as_Transformf();
        break;
        default:
        break;
    }
    return std::unique_ptr<Tool>(newtool);
}

}}