#include "vector3.hpp"

namespace bigrock
{
    namespace math
    {
        Vector3::Vector3(br_real x, br_real y, br_real z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector3::Vector3(const Vector3 &other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }

        #pragma region Operators

        #define OPERATOR_REAL(op) Vector3 Vector3::operator##op(const br_real &val) const\
        {return Vector3(this->x op val, this->y op val, this->z op val);}

        #define OPERATOR_REALREF(op) Vector3 &Vector3::operator##op=(const br_real &val)\
        {this->x op= val; this->y op= val; this->z op= val; return *this;}

        #define OPERATOR_VEC3(op) Vector3 Vector3::operator##op(const Vector3 &other) const\
        {return Vector3(this->x op other.x, this->y op other.y, this->z op other.z);}

        #define OPERATOR_VEC3REF(op) Vector3 &Vector3::operator##op=(const Vector3 &other)\
        {this->x op= other.x;this->y op= other.y;this->z op= other.z;return *this;}

        // VSCode HATES these macros but im lazy

        OPERATOR_REAL(*)
        OPERATOR_REAL(/)

        OPERATOR_REALREF(*)
        OPERATOR_REALREF(/)

        OPERATOR_VEC3(+)
        OPERATOR_VEC3(-)
        OPERATOR_VEC3(*)
        OPERATOR_VEC3(/)

        OPERATOR_VEC3REF(+)
        OPERATOR_VEC3REF(-)
        OPERATOR_VEC3REF(*)
        OPERATOR_VEC3REF(/)

        #undef OPERATOR_REAL
        #undef OPERATOR_REALREF
        #undef OPERATOR_VEC3
        #undef OPERATOR_VEC3REF

        bool Vector3::operator==(const Vector3 &other) const
        {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }

        bool Vector3::operator!=(const Vector3 &other) const
        {
            return this->x != other.x || this->y != other.y || this->z != other.z;
        }

        Vector3 Vector3::operator-()
        {
            return Vector3(-x, -y, -z);
        }

        br_real &Vector3::operator[](const int index)
        {
            switch (index % 3)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
            }
        }

        const br_real &Vector3::operator[](const int index) const
        {
            switch (index % 3)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
            }
        }

        #pragma endregion

        #pragma region Methods

        br_real Vector3::distance_squared_to(const Vector3 &other) const // Gets the distance to another Vector3 without a costly square root calculation
        {
            br_real xdelt, ydelt, zdelt;
            xdelt = (other.x - this->x);
            ydelt = (other.y - this->y);
            zdelt = (other.z - this->z);
            return (xdelt * xdelt) + (ydelt * ydelt) + (zdelt * zdelt);
        }

        br_real Vector3::distance_to(const Vector3 &other) const // Gets the distance between two points. Use this if you NEED precise distance.
        {
            return sqrtf(this->distance_squared_to(other));
        }

        br_real Vector3::length_squared() const // Gets the squared length of the vector (avoids a square root calculation)
        {
            return (x * x) + (y * y) + (z * z);
        }

        br_real Vector3::length() const
        {
            return sqrtf(this->length_squared());
        }

        Vector3 Vector3::abs() const // Returns the absolute form of this vector
        {
            return Vector3(fabsf(this->x), fabsf(this->y), fabsf(this->z));
        }

        Vector3 Vector3::ceil() const
        {
            return Vector3(ceilf(this->x), ceilf(this->y), ceilf(this->z));
        }

        Vector3 Vector3::floor() const
        {
            return Vector3(floorf(this->x), floorf(this->y), floorf(this->z));
        }

        Vector3 Vector3::round() const
        {
            return Vector3(roundf(this->x), roundf(this->y), roundf(this->z));
        }

        Vector3 Vector3::trunc() const
        {
            return Vector3(truncf(this->x), truncf(this->y), truncf(this->z));
        }

        Vector3 Vector3::normalized() const // Sets the vector's length to 1
        {
            if (*this == Vector3::zero)
                return *this;
            else
                return *this / this->length();
        }

        bool Vector3::is_normalized() const // Checks if the length squared is equal to 1
        {
            return std::abs(1.0 - this->length_squared()) < std::numeric_limits<float>::epsilon(); // Approximately equal to 1
        }

        bool Vector3::is_approximately_equal(const Vector3 &other) const // Checks if two Vector3s are within floating-point error range
        {
            return approx_equal(x, other.x) && approx_equal(y, other.y) && approx_equal(z, other.z);
        }

        br_real Vector3::dot(const Vector3 &other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.x);
        }

        Vector3 Vector3::lerp(const Vector3 &other, const float t) const
        {
            if(t <= 0.0)
                return *this;
            else if(t >= 1.0)
                return other;
            
            return (*this) + ((other - *this) * t);
        }

        #pragma endregion

        const Vector3 Vector3::zero = Vector3(0,0,0);
        const Vector3 Vector3::one = Vector3(1,1,1);
    }
}