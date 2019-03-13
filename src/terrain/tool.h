#ifndef BIGROCK_TOOL_H
#define BIGROCK_TOOL_H

#include "../defines.hpp"
#include "../math/vector3.hpp"
#include "../math/transform.hpp"

namespace bigrock
{
    namespace terrain
    {
        struct Tool
        {
            math::Transform transform;

            // Get the isovalue of the point in this tool
            virtual br_real value(const math::Vector3 &point) const = 0;

            // Gets the minimum subdivision level of the tool. Cell subdivision must reach this level when the tool is applied to a cell.
            virtual unsigned int get_min_subdiv(const math::Vector3 &point) const = 0;

            // Gets the maximum subdivision level of the tool. Subdivisions past this level should be considered unnecessary.
            virtual unsigned int get_max_subdiv(const math::Vector3 &point) const = 0;
        };

        struct EllipsoidTool : public Tool
        {
            br_real value(const math::Vector3 &point) const;
            unsigned int get_min_subdiv() const;
            unsigned int get_max_subdiv() const;
        };
    }
}

#endif