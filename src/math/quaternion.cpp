#include "quaternion.hpp"

namespace bigrock
{
    namespace math
    {
		Quaternion::Quaternion()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 1;
		}
		
		Quaternion::Quaternion(const Quaternion &other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
		}

		Quaternion::Quaternion(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Quaternion::Quaternion(const Vector3 &axis, const float radians)
		{
			Vector3 norm;
			if(axis.is_normalized())
				norm = axis;
			else
				norm = axis.normalized();
		
			float sin_half = sinf(radians / 2);
			float cos_half = cosf(radians / 2);

			this->x = axis.x * sin_half;
			this->y = axis.y * sin_half;
			this->z = axis.z * sin_half;
			this->w = cos_half;

			if(!this->is_normalized())
				this->normalize();
		}

		Quaternion::Quaternion(const Vector3 &vector)
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = vector.z;
			this->w = 1;
		}

		Quaternion::Quaternion(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 1;
		}

		// Operators
		Quaternion &Quaternion::operator*=(const Quaternion &other)
		{
			this->w = w * other.w - x * other.x - y * other.y - z * other.z;
			this->x = x * other.w + w * other.x + y * other.z - z * other.y;
			this->y = y * other.w + w * other.y + z * other.x - x * other.z;
			this->z = z * other.w + w * other.z + x * other.y - y * other.x;
			return *this;
		}

		Quaternion Quaternion::operator*(const Quaternion &other) const
		{
			Quaternion ret = *this;
			ret *= other;
			return ret;
		}

		Quaternion &Quaternion::operator*=(const Vector3 &vec)
		{
			this->w = -x * vec.x - y * vec.y - z * vec.z;
			this->x =  w * vec.x + y * vec.z - z * vec.y;
			this->y =  w * vec.y + z * vec.x - x * vec.z;
			this->z =  w * vec.z + x * vec.y - y * vec.x;
			return *this;
		}

		Quaternion Quaternion::operator*(const Vector3 &vec) const
		{
			Quaternion ret = *this;
			ret *= vec;
			return ret;
		}

		Quaternion &Quaternion::operator*=(const float value)
		{
			this->x *= value;
			this->y *= value;
			this->z *= value;
			this->w *= value;
			return *this;
		}

		Quaternion Quaternion::operator*(const float value) const
		{
			Quaternion ret = *this;
			ret *= value;
			return ret;
		}

		bool Quaternion::operator==(const Quaternion &other) const
		{
			return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
		}

		bool Quaternion::operator!=(const Quaternion &other) const
		{
			return (this->x != other.x) || (this->y != other.y) || (this->z != other.z) || (this->w != other.w);
		}

		// Methods
		float Quaternion::length_squared() const
		{
			return (x*x) + (y*y) + (z*z) + (w*w);
		}

		float Quaternion::length() const
		{
			return sqrtf(length_squared());
		}

		bool Quaternion::is_normalized() const
		{
			return approx_equal(length_squared(), 1.0);
		}

		void Quaternion::normalize()
		{
			if(length_squared() == 0 || is_normalized())
				return;
			
			float len = length();
			this->x /= len;
			this->y /= len;
			this->z /= len;
			this->w /= len;
		}

		Quaternion Quaternion::normalized() const
		{
			Quaternion ret = *this;
			ret.normalize();
			return ret;
		}

		Quaternion Quaternion::conjugate() const
		{
			return Quaternion(-x, -y, -z, w);
		}

		Vector3 Quaternion::get_right() const
		{
			return Vector3(
				1.0 - 2.0 * (y*y + z*z),
				2.0       * (x*y - w*z),
				2.0       * (x*z + w*y)
			).normalized();
		}

		Vector3 Quaternion::get_up() const
		{
			return Vector3(
				2.0       * (x*y + w*z),
				1.0 - 2.0 * (x*x + z*z),
				2.0       * (y*z - w*x)
			).normalized();
		}

		Vector3 Quaternion::get_forward() const
		{
			return Vector3(
				2.0       * (x*z - w*y),
				2.0       * (y*z + w*x),
				1.0 - 2.0 * (x*x + y*y)
			).normalized();
		}

		void Quaternion::rotate_axis(const Vector3 &axis, const float radians)
		{
			Quaternion mul = Quaternion(axis, radians);
			*this = mul * (*this); // This feels hacky but lets go with it
		}

		void Quaternion::rotate_axis_degrees(const Vector3 &axis, const float degrees)
		{
			rotate_axis(axis, degrees_to_radians(degrees));
		}

		// Rotates the Quaternion on the X, Y, and Z axes respectively
		// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		void Quaternion::rotate_euler(const Vector3 &radians)
		{
			float cx = cosf(radians.x * 0.5);
			float sx = sinf(radians.x * 0.5);
			float cy = cosf(radians.y * 0.5);
			float sy = sinf(radians.y * 0.5);
			float cz = cosf(radians.z * 0.5);
			float sz = sinf(radians.z * 0.5);

			Quaternion q;
			q.w = cx * cy * cz + sx * sy * sz;
			q.x = cx * cy * sz - sx * sy * cz;
			q.y = sx * cy * sz + cx * sy * cz;
			q.z = sx * cy * cz - cx * sy * sz;
			
			*this = q * (*this);
		}

		Vector3 Quaternion::xform(const Vector3 &point) const
		{
			return (get_right() * point.x) + (get_up() * point.y) + (get_forward() * point.z);
		}

		Vector3 Quaternion::xform_inv(const Vector3 &point) const
		{
			return conjugate().xform(point);
		}

		const Quaternion Quaternion::identity = Quaternion();
    }
}
