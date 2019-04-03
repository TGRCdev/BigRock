#ifndef BIGROCK_TOOL_H
#define BIGROCK_TOOL_H

#include "../../defines.hpp"
#include "../../math/vector3.hpp"
#include "../../math/transform.hpp"
#include "../../math/aabb.hpp"

namespace bigrock
{
    namespace terrain
    {
        struct Tool
        {
            math::Transform transform;

            virtual br_real value(const math::Vector3 &point) const = 0;
            virtual unsigned int get_min_subdiv() const;
            virtual unsigned int get_max_subdiv() const;
            virtual math::AABB get_aabb(const math::Transform &xform = math::Transform()) const = 0;
        };

        struct EllipsoidTool : public Tool
        {
            br_real value(const math::Vector3 &point) const;

            /*
             * Get the AABB of the tool in global coordinates
             * 
             * If xform is defined, gets the AABB relative to
             * the xform provided.
             */
            math::AABB get_aabb(const math::Transform &xform) const;
        };
    }
}

#endif
