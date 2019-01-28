#include "vector3.h"
#include "math_funcs.h"

#include <ostream>

namespace bigrock
{
    // Constructors

    Vector3::Vector3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vector3::Vector3(float x, float y, float z)
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

    // Operators

    #pragma region Operators

    // float operators
    Vector3 Vector3::operator*(const float &val) const
    {
        return Vector3(this->x * val, this->y * val, this->z * val);
    }

    Vector3 Vector3::operator/(const float &val) const
    {
        return Vector3(this->x / val, this->y / val, this->z / val);
    }

    Vector3 &Vector3::operator*=(const float &val)
    {
        this->x *= val;
        this->y *= val;
        this->z *= val;
        return *this;
    }

    Vector3 &Vector3::operator/=(const float &val)
    {
        this->x /= val;
        this->y /= val;
        this->z /= val;
        return *this;
    }

    // Vector3 operators
    Vector3 Vector3::operator+(const Vector3 &other) const
    {
        return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    Vector3 Vector3::operator-(const Vector3 &other) const
    {
        return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    Vector3 Vector3::operator*(const Vector3 &other) const
    {
        return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
    }

    Vector3 Vector3::operator/(const Vector3 &other) const
    {
        return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
    }

    Vector3 &Vector3::operator+=(const Vector3 &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    Vector3 &Vector3::operator-=(const Vector3 &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    Vector3 &Vector3::operator*=(const Vector3 &other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        return *this;
    }

    Vector3 &Vector3::operator/=(const Vector3 &other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        return *this;
    }

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

	float &Vector3::operator[](const int index)
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

	const float &Vector3::operator[](const int index) const
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
    float Vector3::distance_squared_to(const Vector3 &other) const
    {
        float xdelt, ydelt, zdelt;
        xdelt = (other.x - this->x);
        ydelt = (other.y - this->y);
        zdelt = (other.z - this->z);
        return (xdelt * xdelt) + (ydelt * ydelt) + (zdelt * zdelt);
    }

    float Vector3::distance_to(const Vector3 &other) const
    {
        return sqrtf(this->distance_squared_to(other));
    }

    float Vector3::length_squared() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    float Vector3::length() const
    {
        return sqrtf(this->length_squared());
    }

    Vector3 Vector3::abs() const
    {
        return Vector3(fabsf(this->x), fabsf(this->y), fabsf(this->z));
    }

    Vector3 Vector3::normalized() const
    {
		if (*this == Vector3())
			return *this;
		else
			return *this / this->length();
    }

    bool Vector3::is_normalized() const
    {
        return fabsf(float(1.0) - this->length_squared()) < std::numeric_limits<float>::epsilon(); // Approximately equal to 1
    }

    bool Vector3::is_approximately_equal(const Vector3 &other) const
    {
        return MathFuncs::approx_equal(x, other.x) && MathFuncs::approx_equal(y, other.y) && MathFuncs::approx_equal(z, other.z);
    }

    float Vector3::dot(const Vector3 &other) const
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
}
