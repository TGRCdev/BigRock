#include "point.hpp"

#include "../mathfuncs.hpp"

namespace bigrock {

    Point Point::lerp(const Point other, float t) const
    {
        if(t <= 0.0f)
            return *this;
        else if(t >= 1.0f)
            return other;
        else
        {
            if(this->material == other.material) // Same material, lerp density
            {
                return Point(this->material, bigrock::lerp<float>(this->density, other.density, t));
            }
            else if(sign_changes(this->density, other.density)) // Different material, one outside of isosurface
            {
                material_t mat = (this->density >= 0 ? this->material : other.material);
                return Point(mat, bigrock::lerp<float>(this->density, other.density, t));
            }
            else // Different material, both in isosurface
            {
                // Interpolate material
                isovalue_t mat_1 = abs(this->density);
                isovalue_t mat_2 = -abs(other.density);
                isovalue_t mat_true = bigrock::lerp(mat_1, mat_2, t);
                if(mat_true >= 0)
                    return Point(this->material, bigrock::lerp(this->density, other.density, t));
                else
                    return Point(other.material, bigrock::lerp(this->density, other.density, t));
            }
        }
    }

    bool Point::can_collapse(const Point other) const
    {
        if(sign_changes(this->density, other.density)) // Isosurface crossed between points
            return false;
        else if(this->material == other.material) // Same density sign, same material
            return true;
        else if(std::signbit(this->density)) // Same density sign, different material, points are outside isosurface
            return true;
        else // Same density sign, different material, points are inside isosurface
            return false;
    }
}