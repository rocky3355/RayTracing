#include <cmath>
#include "vector3.h"

namespace raytracing
{
Vector3::Vector3()
{
	data[0] = 0.0;
	data[1] = 0.0;
	data[2] = 0.0;
}

Vector3::Vector3(double x, double y, double z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-data[0], -data[1], -data[2]);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	data[0] += v.data[0];
	data[1] += v.data[1];
	data[2] += v.data[2];
	return *this;
}

Vector3& Vector3::operator*=(const double t)
{
	data[0] *= t;
	data[1] *= t;
	data[2] *= t;
	return *this;
}

Vector3& Vector3::operator/=(const double t)
{
	data[0] /= t;
	data[1] /= t;
	data[2] /= t;
	return *this;
}

double Vector3::Dot(const Vector3& v) const
{
	return data[0] * v.data[0]
		+ data[1] * v.data[1]
		+ data[2] * v.data[2];
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(data[1] * v.data[2] - data[2] * v.data[1],
		data[2] * v.data[0] - data[0] * v.data[2],
		data[0] * v.data[1] - data[1] * v.data[0]);
}

Vector3 Vector3::UnitVector() const
{
	return (*this / GetLength());
}

double Vector3::GetLength() const
{
	return std::sqrt(GetLengthSquared());
}

double Vector3::GetLengthSquared() const
{
	return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
}

Vector3 Vector3::Reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * v.Dot(n) * n;
}

Vector3 Vector3::Refract(const Vector3& uv, const Vector3& n, double etai_over_etat)
{
	double cos_theta = std::fmin(-uv.Dot(n), 1.0);
	Vector3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	Vector3 r_out_perp = -std::sqrt(1.0 - r_out_parallel.GetLengthSquared()) * n;
	return r_out_parallel + r_out_perp;
}

Vector3 Vector3::GetRandomInUnitSphere()
{
	while (true)
	{
		Vector3 p = GetRandom(-1.0, 1.0);
		if (p.GetLengthSquared() >= 1.0)
		{
			continue;
		}
		return p;
	}
}

Vector3 Vector3::GetRandomUnitVector()
{
	double a = GetRandomDouble(0, 2 * M_PI);
	double z = GetRandomDouble(-1.0, 1.0);
	double r = std::sqrt(1.0 - z * z);
	return Vector3(r * std::cos(a), r * std::sin(a), z);
}

Vector3 Vector3::GetRandomInHemisphere(const Vector3& normal)
{
	Vector3 in_unit_sphere = GetRandomInUnitSphere();
	if (in_unit_sphere.Dot(normal) > 0.0)
	{
		return in_unit_sphere;
	}
	return -in_unit_sphere;
}

Vector3 Vector3::GetRandomInUnitDisk()
{
	while (true)
	{
		Vector3 point = Vector3(GetRandomDouble(-1.0, 1.0), GetRandomDouble(-1.0, 1.0), 0.0);
		if (point.GetLengthSquared() >= 1.0)
		{
			continue;
		}
		return point;
	}
}

Vector3 Vector3::GetRandomToSphere(double radius, double distance_squared)
{
	double r1 = GetRandomDouble();
	double r2 = GetRandomDouble();
	double z = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

	double phi = 2 * M_PI * r1;
	double x = std::cos(phi) * std::sqrt(1 - z * z);
	double y = std::sin(phi) * std::sqrt(1 - z * z);

	return Vector3(x, y, z);
}

Vector3 Vector3::GetRandomCosineDirection()
{
	double r1 = GetRandomDouble();
	double r2 = GetRandomDouble();
	double z = std::sqrt(1.0 - r2);

	double phi = 2.0 * M_PI * r1;
	double x = std::cos(phi) * std::sqrt(r2);
	double y = std::sin(phi) * std::sqrt(r2);

	return Vector3(x, y, z);
}
}  // namespace raytracing
