#ifndef BIGROCK_QUATERNION_H
#define BIGROCK_QUATERNION_H

#include <math.h>

#include "vector3.hpp"
#include "math_funcs.hpp"

namespace bigrock
{
	namespace math
	{
		class Quaternion
		{
			public:
			float x, y, z, w;

			// Constructors
			Quaternion();
			
			Quaternion(const Quaternion &other);

			Quaternion(float x, float y, float z, float w);

			Quaternion(const Vector3 &axis, const float radians);

			Quaternion(const Vector3 &vector);

			Quaternion(float x, float y, float z);

			// Operators
			Quaternion &operator*=(const Quaternion &other);

			Quaternion operator*(const Quaternion &other) const;

			Quaternion &operator*=(const Vector3 &vec);

			Quaternion operator*(const Vector3 &vec) const;

			Quaternion &operator*=(const float value);

			Quaternion operator*(const float value) const;

			bool operator==(const Quaternion &other) const;

			bool operator!=(const Quaternion &other) const;

			// Methods
			float length_squared() const;

			float length() const;

			bool is_normalized() const;

			void normalize();

			Quaternion normalized() const;

			Quaternion conjugate() const;

			Vector3 get_right() const;

			Vector3 get_up() const;

			Vector3 get_forward() const;

			void rotate_axis(const Vector3 &axis, const float radians);

			void rotate_axis_degrees(const Vector3 &axis, const float degrees);

			// Rotates the Quaternion on the X, Y, and Z axes respectively
			// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
			void rotate_euler(const Vector3 &radians);

			Vector3 xform(const Vector3 &point) const;

			Vector3 xform_inv(const Vector3 &point) const;
		};
	}
}

#endif
