#ifndef BIGROCK_EMPLACE_ACTION_H
#define BIGROCK_EMPLACE_ACTION_H

#include "action.hpp"

namespace bigrock {
namespace terrain {
namespace data {

struct EmplaceAction : public Action
{
    EmplaceAction(const unsigned int &material = 0, const unsigned int &max_subdiv_level = br_default_subdiv, const bool &remove = false);

    unsigned int material;
    bool remove;

    void update(const Tool &t, CellVertex &v) const;
};

}
}
}

#endif
