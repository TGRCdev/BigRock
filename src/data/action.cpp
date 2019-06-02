#include "action.hpp"

#include "actions/emplace.hpp"

#include "schemas/action_generated.h"

namespace bigrock {
namespace data {

std::string Action::serialize() const
{
    schemas::ActionType atype;
    switch(get_action_type())
    {
        case ACTION_TYPE_EMPLACE:
        atype = schemas::ActionType_Emplace;
        break;
        default:
        return std::string();
    }
    flatbuffers::FlatBufferBuilder b;
    auto action = schemas::CreateAction(b, atype);
    b.Finish(action);
    return std::string(reinterpret_cast<char*>(b.GetBufferPointer()), static_cast<size_t>(b.GetSize()));
}

std::unique_ptr<Action> Action::deserialize(const void *buf, size_t length)
{
    if(buf == NULL)
        return nullptr;
    flatbuffers::Verifier v = flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(buf), length);
    bool ok = schemas::VerifyActionBuffer(v);
    if(!ok)
        return nullptr;
    
    auto action = schemas::GetAction(buf);
    Action *newact = NULL;
    switch(action->type())
    {
        case schemas::ActionType_Emplace:
        newact = new actions::Emplace();
        break;
    }

    if(!newact)
        return nullptr;
    
    return std::unique_ptr<Action>(newact);
}

}}