#ifndef BIGROCK_ACTION_H
#define BIGROCK_ACTION_H

#include "../../defines.hpp"
#include "../../math/vector3.hpp"

namespace bigrock
{
    namespace terrain
    {
        struct Tool;
        struct Vertex;

        struct Action
        {
            virtual void update(const Tool &t, Vertex &v) const = 0;
        };

        struct AddAction : public Action
        {
            br_real amount;
            void update(const Tool &t, Vertex &v) const;
        };

        struct EmplaceAction : public Action
        {
            unsigned int material;
            
            void update(const Tool &t, Vertex &v) const;
        };

        struct DeleteAction : public Action
        {
            void update(const Tool &t, Vertex &v) const;
        };
    }
}

#endif
