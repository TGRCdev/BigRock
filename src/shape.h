#ifndef BIGROCK_SHAPE_H
#define BIGROCK_SHAPE_H

#include "transform.h"
#include "vector3.h"
#include "ray.h"

namespace bigrock
{
    // Represents convex shapes
    // P.S. I am not implementing concave shapes
    class Shape
    {
        public:
        Transform transform;

        virtual bool contains_point(const Vector3 &point) const = 0;
        virtual Vector3 get_closest_point(const Vector3 &target) const = 0;
    };

    class Sphere : public Shape
    {
        public:
        float radius;

        bool contains_point(const Vector3 &point) const;
        Vector3 get_closest_point(const Vector3 &target) const;
    };

    class Rectangle : public Shape
    {
        public:
        Vector3 size;
        
        bool contains_point(const Vector3 &point) const;
        Vector3 get_closest_point(const Vector3 &target) const;
    };

    // Some important notes:
    // height is actually the distance between the origins of the two sphere at the ends
    // so the true height of a capsule is (radius * 2) + height
    class Capsule : public Sphere
    {
        public:
        float height; // Please don't set this to negative

        float get_actual_height() const;
        void set_actual_height(const float height);

        bool contains_point(const Vector3 &point) const;
        Vector3 get_closest_point(const Vector3 &target) const;
    };

    // Basically a Capsule but without the spheres at the ends.
    class Cylinder : public Sphere
    {
        public:
        float height; // Please don't set this to negative

        bool contains_point(const Vector3 &point) const;
        Vector3 get_closest_point(const Vector3 &target) const;
    };
}

#endif
