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
    if(!remove)
    {
        if(val > 0 || (val > -1 && v.isovalue < val)) // Vertex inside tool
        {
            if(val > v.isovalue)
                v.isovalue = val;
            if(val > 0)
                v.material = material;
        }
    }
    else
        if(-val < v.isovalue)
            v.isovalue = -val;
}

}
}
}