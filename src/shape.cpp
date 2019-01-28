#include "shape.h"
#include "ray.h"

#include <algorithm>

namespace bigrock
{
    bool Sphere::contains_point(const Vector3 &point) const
    {
        return transform.to_local(point).length_squared() <= (radius * radius); 
    }

    Vector3 Sphere::get_closest_point(const Vector3 &target) const
    {
        Vector3 local = transform.to_local(target);
        Ray ray(Vector3(), local);

        float length;
        if(local.length_squared() < (radius * radius))
            length = target.length();
        else
            length = radius;
        
        return transform.to_global(ray.get_point_on_ray(length));
    }

    bool Rectangle::contains_point(const Vector3 &point) const
    {
        // Convert the point to local coordinates
        Vector3 local = transform.to_local(point);

        Vector3 bounds = transform.origin + size;

        return (local.x >= transform.origin.x && local.y >= transform.origin.y && local.z >= transform.origin.z && local.x <= bounds.x && local.y <= bounds.y && local.z <= bounds.z);
    }

    Vector3 Rectangle::get_closest_point(const Vector3 &target) const
    {
        Vector3 local = transform.to_local(target);

        Vector3 closest;
        if(local.x <= 0.0)
            closest.x = 0.0;
        else if(local.x >= size.x)
            closest.x = size.x;
        else
            closest.x = local.x;
        
        if(local.y <= 0.0)
            closest.y = 0.0;
        else if(local.y >= size.y)
            closest.y = size.y;
        else
            closest.y = local.y;
        
        if(local.z <= 0.0)
            closest.z = 0.0;
        else if(local.z >= size.z)
            closest.z = size.z;
        else
            closest.z = local.z;
        
        return transform.to_global(closest);
    }

    float Capsule::get_actual_height() const
    {
        return (radius * 2) + height;
    }

    void Capsule::set_actual_height(const float height)
    {
        this->height = std::max((height - (radius * 2)), float(0));
    }

    bool Capsule::contains_point(const Vector3 &point) const
    {
        Vector3 local = transform.to_local(point);
        
        // First, check sphere at the bottom
        if(local.distance_squared_to(Vector3(0,0,0)) <= (radius * radius))
            return true;
        
        // Next, check sphere at the top
        if(local.distance_squared_to(Vector3(0, height, 0)) <= (radius * radius))
            return true;
        
        // Finally, check if the point is within the cylinder
        if(local.y >= 0.0 && local.y <= height)
            return Vector3(local.x, 0, local.z).distance_squared_to(Vector3(0, 0, 0)) < (radius * radius);
        else
            return false;
    }

    Vector3 Capsule::get_closest_point(const Vector3 &target) const
    {
        Vector3 local = transform.to_local(target);

        Ray ray;
        if(local.y <= 0.0)
        { // Check bottom sphere
            ray = Ray(Vector3(0,0,0), local);
        }
        else if(local.y >= height)
        { // Check top sphere
            ray = Ray(Vector3(0, height, 0), local - Vector3(0, height, 0));
        }
        else
        { // Check cylinder
            ray = Ray(Vector3(0, local.y, 0), local - Vector3(0, local.y, 0));
        }

        float length;
        if(local.distance_squared_to(ray.origin) <= (radius * radius))
            length = local.distance_to(ray.origin);
        else
            length = radius;
        return transform.to_global(ray.get_point_on_ray(length));
    }

    bool Cylinder::contains_point(const Vector3 &point) const
    {
        Vector3 local = transform.to_local(point);
        if(local.y < transform.origin.y || local.y > transform.origin.y + height)
            return false;
        
        Vector3 bottom = Vector3(transform.origin.x, 0, transform.origin.z);
        local.y = 0;

        return bottom.distance_squared_to(local) <= (radius * radius);
    }

    Vector3 Cylinder::get_closest_point(const Vector3 &target) const
    {
        Vector3 local = transform.to_local(target);
        Vector3 local_bottom = Vector3(local.x, 0, local.z);

        Ray ray(Vector3(0, 0, 0), local_bottom);

        float length;
        if(local_bottom.distance_squared_to(Vector3(0,0,0)) <= (radius * radius))
            length = local_bottom.distance_to(Vector3(0,0,0));
        else
            length = radius;
        
        Vector3 add = Vector3(0,0,0);
        if(local.y > 0)
        {
            if(local.y >= height)
                add.y = height;
            else
                add.y = local.y;
        }

        return transform.to_global(ray.get_point_on_ray(length) + add);
    }
}
