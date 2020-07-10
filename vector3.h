#pragma once

#include <iostream>
#include "utilities.h"

namespace raytracing
{
class Vector3
{
public:
	Vector3();
	Vector3(double x, double y, double z);
	Vector3 operator-() const;
	double operator[](int idx) const;
	double& operator[](int idx);
	Vector3& operator+=(const Vector3& v);
	Vector3& operator*=(const double t);
	Vector3& operator/=(const double t);

	// TODO: Inline?
	inline double x() const { return data[0]; }
	inline double y() const { return data[1]; }
	inline double z() const { return data[2]; }
	double GetLength() const;
	double GetLengthSquared() const;
	double Dot(const Vector3& v) const;
	Vector3 Cross(const Vector3& v) const;
	Vector3 UnitVector() const;
	static Vector3 Reflect(const Vector3& v, const Vector3& n);
	static Vector3 Refract(const Vector3& uv, const Vector3& n, double etai_over_etat);
	static Vector3 GetRandomInUnitSphere();
	static Vector3 GetRandomUnitVector();
	static Vector3 GetRandomInHemisphere(const Vector3& normal);
	static Vector3 GetRandomInUnitDisk();
	static Vector3 GetRandomToSphere(double radius, double distance_squared);

	inline static Vector3 GetRandom()
	{
		return Vector3(GetRandomDouble(), GetRandomDouble(), GetRandomDouble());
	}

	inline static Vector3 GetRandom(double min, double max) 
	{
		return Vector3(GetRandomDouble(min, max), GetRandomDouble(min, max), GetRandomDouble(min, max));
	}

	inline static Vector3 GetRandomCosineDirection()
	{
		double r1 = GetRandomDouble();
		double r2 = GetRandomDouble();
		double z = std::sqrt(1.0 - r2);

		double phi = 2.0 * M_PI * r1;
		double x = std::cos(phi) * std::sqrt(r2);
		double y = std::sin(phi) * std::sqrt(r2);

		return Vector3(x, y, z);
	}

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
