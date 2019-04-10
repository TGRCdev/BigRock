#include "emplace_action.hpp"

namespace bigrock {
namespace terrain {
namespace data {

EmplaceAction::EmplaceAction(const unsigned int &material, const unsigned int &max_subdiv_level, const bool &remove)
{
    this->material = material;
    this->max_subdiv_level = max_subdiv_level;
    this->remove = remove;
}

void EmplaceAction::update(const Tool &t, CellVertex &v) const
{
    br_real val = t.value(v.position);
    if(remove)
    {
        if(v.isovalue > 0) // Vertex inside tool
            v.isovalue = val;
        else if(val > -1) // Vertex within influence
            v.isovalue = (v.isovalue < -val ? v.isovalue : -val);
    }
    else
        v.isovalue = val;
}

}
}
}