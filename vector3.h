#ifndef VECTOR3_H
#define VECTOR3_H

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
	double GetLength() const;
	double GetLengthSquared() const;

	// TODO: Forward declaration not possible, as ostream is an alias?
	inline std::ostream& operator<<(std::ostream& out);
	inline Vector3 operator+(const Vector3& v);
	inline Vector3 operator-(const Vector3& v);
	inline Vector3 operator*(const Vector3& v);
	inline Vector3 operator*(double t);
	inline Vector3 operator/(double t);
	inline double Dot(const Vector3& v);
	inline Vector3 Cross(const Vector3& v);
	inline Vector3 UnitVector();

	double data[3];
};

using Point3 = Vector3;
using Color = Vector3;

inline Vector3 operator*(double t, const Vector3& v) {
	return Vector3(t * v.data[0], t * v.data[1], t * v.data[2]);
}

}  // raytracing

#endif  // VECTOR3_H
