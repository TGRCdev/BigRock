#ifndef BIGROCK_VECTOR3_H
#define BIGROCK_VECTOR3_H

// TODO: Split into hpp and cpp

#include "../defines.hpp"
#include "math_funcs.hpp"

#include <cmath>

namespace bigrock
{
    namespace math
    {
        struct Vector3;

        struct Vector3
        {
            br_real x, y, z;

            // Constructors

            Vector3(br_real x = 0, br_real y = 0, br_real z = 0);
            Vector3(const Vector3 &other);

            #pragma region Operators

            Vector3 operator*(const br_real &val) const;
            Vector3 operator/(const br_real &val) const;
            Vector3 &operator*=(const br_real &val);
            Vector3 &operator/=(const br_real &val);

            // Vector3 operators
            Vector3 operator+(const Vector3 &other) const;
            Vector3 operator-(const Vector3 &other) const;
            Vector3 operator*(const Vector3 &other) const;
            Vector3 operator/(const Vector3 &other) const;

            Vector3 &operator+=(const Vector3 &other);
            Vector3 &operator-=(const Vector3 &other);
            Vector3 &operator*=(const Vector3 &other);
            Vector3 &operator/=(const Vector3 &other);

            // Other operators

            bool operator==(const Vector3 &other) const;
            bool operator!=(const Vector3 &other) const;

            Vector3 operator-();

            br_real &operator[](const int index);
            const br_real &operator[](const int index) const;

            #pragma endregion

            #pragma region Methods

            br_real distance_squared_to(const Vector3 &other) const; // Gets the distance to another Vector3 without a costly square root calculation
            br_real distance_to(const Vector3 &other) const; // Gets the distance between two points. Use this if you NEED precise distance.

            br_real length_squared() const; // Gets the squared length of the vector (avoids a square root calculation)
            br_real length() const;

            Vector3 abs() const; // Returns the absolute form of this vector
            Vector3 ceil() const;
            Vector3 floor() const;
            Vector3 round() const;
            Vector3 trunc() const;

            Vector3 normalized() const; // Sets the vector's length to 1
            bool is_normalized() const; // Checks if the length squared is equal to 1

            bool is_approximately_equal(const Vector3 &other) const; // Checks if two Vector3s are within floating-point error range
            br_real dot(const Vector3 &other) const;
            Vector3 lerp(const Vector3 &other, const float t) const;

            static const Vector3 zero;
            static const Vector3 one;
            #pragma endregion
        };
    }
}

#endif
