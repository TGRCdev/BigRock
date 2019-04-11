#include "ray.hpp"

namespace bigrock
{
    namespace math
    {
        Ray::Ray(const Vector3 &origin, const Vector3 &direction)
        {
            this->origin = origin;
            this->set_direction(direction);
        }

        void Ray::set_direction(const Vector3 &direction)
        {
            if(direction.is_normalized())
                this->direction = direction;
            else
                this->direction = direction.normalized();
        }

        Vector3 Ray::get_point_on_ray(const br_real t) const
        {
            return origin + (direction * t);
        }

        Vector3 Ray::get_closest_point(const Vector3 &target) const
        {
            Vector3 v = target - origin;
            Vector3 d = direction * v;
            return origin + direction * d;
        }
    }
}
