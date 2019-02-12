#include "vector3i.hpp"

namespace bigrock
{
    namespace math
    {
        #pragma region Constructors
        Vector3i::Vector3i()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        Vector3i::Vector3i(int x, int y, int z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector3i::Vector3i(const Vector3i &other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }

        Vector3i::Vector3i(const Vector3 &other)
        {
            this->x = int(other.x);
            this->y = int(other.y);
            this->z = int(other.z);
        }
        #pragma endregion
        #pragma region Methods
        
        #pragma endregion
        #pragma region Operators

        Vector3i::operator Vector3() const
        {
            return Vector3(x, y, z);
        }

        bool Vector3i::operator==(const Vector3i &other) const
        {
            return (this->x == other.x && this->y == other.y && this->z == other.z);
        }

        bool Vector3i::operator!=(const Vector3i &other) const
        {
            return (this->x != other.x || this->y != other.y || this->z != other.z);
        }

        Vector3i Vector3i::operator+(const Vector3i &other) const
        {
            return Vector3i(this->x + other.x, this->y + other.y, this->z + other.z);
        }

        Vector3i &Vector3i::operator+=(const Vector3i &other)
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        Vector3i Vector3i::operator-(const Vector3i &other) const
        {
            return Vector3i(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        Vector3i &Vector3i::operator-=(const Vector3i &other)
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            return *this;
        }

        Vector3i Vector3i::operator%(const int val) const
        {
            return Vector3i(x % val, y % val, z % val);
        }

        Vector3i &Vector3i::operator%=(const int val)
        {
            this->x %= val;
            this->y %= val;
            this->z %= val;
            return *this;
        }

        #pragma endregion
    }
}