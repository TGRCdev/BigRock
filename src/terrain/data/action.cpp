#include "action.hpp"

#include "tool.hpp"
#include "terrain.hpp"
#include <math.h>

namespace bigrock
{
    namespace terrain
    {
        void AddAction::update(const Tool &t, Vertex &v) const
        {
            if(t.value(v.location) > 0)
                v.value = fmin(3.0, fmin(0.0, v.value + amount));
        }

        void EmplaceAction::update(const Tool &t, Vertex &v) const
        {
            br_real val = t.value(v.location);
            if(val >= 1.5)
            {
                v.value = val;
                v.material = material;
            }
        }

        void DeleteAction::update(const Tool &t, Vertex &v) const
        {
            br_real val = t.value(v.location);
            if(val >= 1.5)
            {
                v.value = val;
                v.material = -1;
            }
        }
    }
}
