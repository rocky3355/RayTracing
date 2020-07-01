#include <iostream>
#include <fstream>
#include "ray3.h"

using namespace raytracing;

bool hit_sphere(const Vector3& center, double radius, const Ray3& r)
{
	Vector3 oc = r.origin - center;
	auto a = r.direction.Dot(r.direction);
	auto b = 2.0 * oc.Dot(r.direction);
	auto c = oc.Dot(oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

Vector3 RayColor(const Ray3& r)
{
	if (hit_sphere(Vector3(0, 0, -1), 0.5, r))
		return Vector3(1, 0, 0);
	Vector3 unit_direction = r.direction.UnitVector();
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
}

void WriteColor(std::ostream& out, const Vector3& pixel_color)
{
	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

int main()
{	
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	const double viewport_height = 2.0;
	const double viewport_width = aspect_ratio * viewport_height;
	const double focal_length = 1.0;

	const Vector3 origin = Vector3(0.0, 0.0, 0.0);
	const Vector3 horizontal = Vector3(viewport_width, 0.0, 0.0);
	const Vector3 vertical = Vector3(0, viewport_height, 0.0);
	const Vector3 lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - Vector3(0, 0, focal_length);

	std::ofstream file;
	file.open("test.ppm");
	file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = image_height - 1; y >= 0; --y)
	{
		std::cout << "Scanlines remaining: " << y << std::endl;
		for (int x = 0; x < image_width; ++x)
		{
			auto u = double(x) / (image_width - 1);
			auto v = double(y) / (image_height - 1);
			Ray3 r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			Vector3 pixel_color = RayColor(r);
			WriteColor(file, pixel_color);
		}
	}
}
