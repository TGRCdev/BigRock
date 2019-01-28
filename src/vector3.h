#ifndef BIGROCK_VECTOR3_H
#define BIGROCK_VECTOR3_H

#include <iostream>

namespace bigrock
{
    struct Vector3
    {
        float x, y, z;

        // Constructors

        Vector3();
        Vector3(float x, float y, float z);
        Vector3(const Vector3 &other);

        #pragma region Operators

        // float operators
        Vector3 operator*(const float &val) const;
        Vector3 operator/(const float &val) const;

        Vector3 &operator*=(const float &val);
        Vector3 &operator/=(const float &val);

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

		float &operator[](const int index);
        const float &operator[](const int index) const;

        friend std::ostream &operator<<(std::ostream &out, const Vector3 &obj)
        {
            out << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
            return out;
        }

        #pragma endregion

        #pragma region Methods

        float distance_squared_to(const Vector3 &other) const; // Gets the distance to another Vector3 without a costly square root calculation
        float distance_to(const Vector3 &other) const; // Gets the distance between two points. Use this if you NEED precise distance.

        float length_squared() const; // Gets the squared length of the vector (avoids a square root calculation)
        float length() const;

        Vector3 abs() const; // Returns the absolute form of this vector
        Vector3 normalized() const; // Sets the vector's length to 1

        bool is_normalized() const; // Checks if the length squared is equal to 1
        bool is_approximately_equal(const Vector3 &other) const; // Checks if two Vector3s are within floating-point error range

        float dot(const Vector3 &other) const;

        Vector3 lerp(const Vector3 &other, const float t) const;

        #pragma endregion
    };
}

#endif
