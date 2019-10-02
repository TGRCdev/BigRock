#include "emplace.hpp"

namespace bigrock {

    void Emplace::update(const Shape &shape, glm::vec3 position, Point &point) const
    {
        isovalue_t value = shape.get_value(position);
        if(value >= 0)
            point.material = material;
        
        if(value > point.density)
            point.density = value;
    }

}