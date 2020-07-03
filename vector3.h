#pragma once

#include <iostream>

namespace raytracing
{
class Vector3
{
public:
	Vector3();
	Vector3(double x, double y, double z);

	double x() const;
	double y() const;
	double z() const;
	Vector3 operator-() const;
	double operator[](int idx) const;
	double& operator[](int idx);
	Vector3& operator+=(const Vector3& v);
	Vector3& operator*=(const double t);
	Vector3& operator/=(const double t);

	// TODO: Inline?
	double GetLength() const;
	double GetLengthSquared() const;
	double Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;
	Vector3 UnitVector() const;

	double data[3];
};

// TODO: Move this inside class
static const Vector3 VECTOR3_ZERO(0.0, 0.0, 0.0);
static const Vector3 VECTOR3_UNIT_XYZ(1.0, 1.0, 1.0);

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2];
}

inline Vector3 operator*(double t, const Vector3& v)
{
	return Vector3(t * v.data[0], t * v.data[1], t * v.data[2]);
}

inline Vector3 operator*(const Vector3& v, double t)
{
	return Vector3(t * v.data[0], t * v.data[1], t * v.data[2]);
}

inline Vector3 operator/(const Vector3& v, double t)
{
	return (1 / t) * v;
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
	return Vector3(u.data[0] + v.data[0], u.data[1] + v.data[1], u.data[2] + v.data[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
	return Vector3(u.data[0] - v.data[0], u.data[1] - v.data[1], u.data[2] - v.data[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
	return Vector3(u.data[0] * v.data[0], u.data[1] * v.data[1], u.data[2] * v.data[2]);
}
}  // namespace raytracing

//#endif  // VECTOR3_H
