#include "vector3.h"
#include <cmath>

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

double Vector3::x() const
{
	return data[0];
}

double Vector3::y() const
{
	return data[1];
}

double Vector3::z() const
{
	return data[2];
}

Vector3 Vector3::operator-() const
{
	return Vector3(-data[0], -data[1], -data[2]);
}

double Vector3::operator[](int idx) const
{
	return data[idx];
}

double& Vector3::operator[](int idx)
{
	return data[idx];
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
}  // namespace raytracing
