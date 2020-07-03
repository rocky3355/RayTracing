#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hittable_list.h"

using namespace raytracing;

Vector3 RayColor(const Ray3& ray, const Hittable& object)
{
	HitRecord hit_record;
	if (object.Hit(ray, 0.0, 1000.0, hit_record))
	{
		return 0.5 * (VECTOR3_UNIT_XYZ + hit_record.normal);
	}

	Vector3 unit_direction = ray.direction.UnitVector();
	double t = 0.5 * (unit_direction.y() + 1.0);
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
	HittableList world;
	world.Add(std::make_shared<Sphere>(Vector3(0, 0, -1), 0.5));
	world.Add(std::make_shared<Sphere>(Vector3(0, -100.5, -1), 100));

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
			Ray3 ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			Vector3 pixel_color = RayColor(ray, world);
			WriteColor(file, pixel_color);
		}
	}
}
