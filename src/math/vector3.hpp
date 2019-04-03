#ifndef BIGROCK_VECTOR3_H
#define BIGROCK_VECTOR3_H

// TODO: Split into hpp and cpp

#include "../defines.hpp"
#include "math_funcs.hpp"

#include <boost/container_hash/hash.hpp>

#include <cmath>
#include <stdexcept>

namespace bigrock
{
    namespace math
    {
        struct Vector3;

        struct Vector3
        {
            br_real x, y, z;

            // Constructors

            Vector3()
            {
                this->x = 0;
                this->y = 0;
                this->z = 0;
            }

            Vector3(br_real x, br_real y, br_real z)
            {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            Vector3(const Vector3 &other)
            {
                this->x = other.x;
                this->y = other.y;
                this->z = other.z;
            }

            #pragma region Operators

            // T operators
            Vector3 operator*(const br_real &val) const
            {
                return Vector3(this->x * val, this->y * val, this->z * val);
            }

            Vector3 operator/(const br_real &val) const
            {
                return Vector3(this->x / val, this->y / val, this->z / val);
            }

            Vector3 &operator*=(const br_real &val)
            {
                this->x *= val;
                this->y *= val;
                this->z *= val;
                return *this;
            }

            Vector3 &operator/=(const br_real &val)
            {
                this->x /= val;
                this->y /= val;
                this->z /= val;
                return *this;
            }

            // Vector3 operators
            Vector3 operator+(const Vector3 &other) const
            {
                return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
            }

            Vector3 operator-(const Vector3 &other) const
            {
                return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
            }

            Vector3 operator*(const Vector3 &other) const
            {
                return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
            }

            Vector3 operator/(const Vector3 &other) const
            {
                return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
            }

            Vector3 &operator+=(const Vector3 &other)
            {
                this->x += other.x;
                this->y += other.y;
                this->z += other.z;
                return *this;
            }

            Vector3 &operator-=(const Vector3 &other)
            {
                this->x -= other.x;
                this->y -= other.y;
                this->z -= other.z;
                return *this;
            }

            Vector3 &operator*=(const Vector3 &other)
            {
                this->x *= other.x;
                this->y *= other.y;
                this->z *= other.z;
                return *this;
            }

            Vector3 &operator/=(const Vector3 &other)
            {
                this->x /= other.x;
                this->y /= other.y;
                this->z /= other.z;
                return *this;
            }

            // Other operators

            bool operator==(const Vector3 &other) const
            {
                return this->x == other.x && this->y == other.y && this->z == other.z;
            }

            bool operator!=(const Vector3 &other) const
            {
                return this->x != other.x || this->y != other.y || this->z != other.z;
            }

            Vector3 operator-()
            {
                return Vector3(-x, -y, -z);
            }

            br_real &operator[](const int index)
            {
                switch (index)
                {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    throw std::out_of_range("Attempted to access out of range element on Vector3");
                }
            }

            const br_real &operator[](const int index) const
            {
                switch (index)
                {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    throw std::out_of_range("Attempted to access out of range element on Vector3");
                }
            }

            #pragma endregion

            #pragma region Methods

            br_real distance_squared_to(const Vector3 &other) const // Gets the distance to another Vector3 without a costly square root calculation
            {
                br_real xdelt, ydelt, zdelt;
                xdelt = (other.x - this->x);
                ydelt = (other.y - this->y);
                zdelt = (other.z - this->z);
                return (xdelt * xdelt) + (ydelt * ydelt) + (zdelt * zdelt);
            }

            br_real distance_to(const Vector3 &other) const // Gets the distance between two points. Use this if you NEED precise distance.
            {
                return sqrtf(this->distance_squared_to(other));
            }

            br_real length_squared() const // Gets the squared length of the vector (avoids a square root calculation)
            {
                return (x * x) + (y * y) + (z * z);
            }

            br_real length() const
            {
                return sqrtf(this->length_squared());
            }

            Vector3 abs() const // Returns the absolute form of this vector
            {
                return Vector3(fabsf(this->x), fabsf(this->y), fabsf(this->z));
            }

            Vector3 ceil() const
            {
                return Vector3(ceilf(this->x), ceilf(this->y), ceilf(this->z));
            }

            Vector3 floor() const
            {
                return Vector3(floorf(this->x), floorf(this->y), floorf(this->z));
            }

            Vector3 round() const
            {
                return Vector3(roundf(this->x), roundf(this->y), roundf(this->z));
            }

            Vector3 trunc() const
            {
                return Vector3(truncf(this->x), truncf(this->y), truncf(this->z));
            }

            Vector3 normalized() const // Sets the vector's length to 1
            {
                if (*this == Vector3())
                    return *this;
                else
                    return *this / this->length();
            }

            bool is_normalized() const // Checks if the length squared is equal to 1
            {
                return std::abs(1.0 - this->length_squared()) < std::numeric_limits<float>::epsilon(); // Approximately equal to 1
            }

            bool is_approximately_equal(const Vector3 &other) const // Checks if two Vector3s are within floating-point error range
            {
                return approx_equal(x, other.x) && approx_equal(y, other.y) && approx_equal(z, other.z);
            }

            br_real dot(const Vector3 &other) const
            {
                return (x * other.x) + (y * other.y) + (z * other.x);
            }

            Vector3 lerp(const Vector3 &other, const float t) const
            {
                if(t <= 0.0)
                    return *this;
                else if(t >= 1.0)
                    return other;
                
                return (*this) + ((other - *this) * t);
            }

            static const Vector3 zero;
            #pragma endregion
        };

        std::size_t hash_value(const bigrock::math::Vector3 &vec);
    }
}

#endif
