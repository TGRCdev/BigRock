#ifndef BIGROCK_ACTION_H
#define BIGROCK_ACTION_H

#include "cell_vertex.hpp"
#include "tool.hpp"

namespace bigrock {
namespace terrain {
namespace data {

struct Action
{
    unsigned int max_subdiv_level;

    virtual void update(const Tool &t, CellVertex &v) const = 0;
};

}
}
}

#endif