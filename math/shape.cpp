#include "shape.hpp"

namespace bigrock
{
    namespace math
    {
        float Sphere::get_isovalue(const Vector3 &point, const float margin) const
        {
            float dist_sqr = transform.to_local(point).length_squared();
            if(dist_sqr <= (radius * radius))
            { // Point is inside of sphere
                if(dist_sqr <= (radius - margin) * (radius - margin))
                { // Point is distinctly inside of sphere and margin
                    return 1.0;
                }
                else
                { // dist_sqr is between radius^2 and (radius-margin)^2
                    return margin - (sqrtf(dist_sqr) - (radius-margin));
                }
            }
            else
            { // Point is outside of sphere
                if(dist_sqr >= (radius + margin) * (radius + margin))
                { // Point is outside the margin
                    return -1.0;
                }
                else
                {
                    return sqrt(dist_sqr) - (radius+margin);
                }
            }
        }

        bool Sphere::contains_point(const Vector3 &point) const
        {
            return point.distance_squared_to(transform.origin) <= (radius * radius);
        }
    }
}