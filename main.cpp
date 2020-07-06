#include <iostream>
#include <fstream>
#include <thread>
#include "sphere.h"
#include "camera.h"
#include "lambertian_material.h"
#include "dielectric_material.h"
#include "metal_material.h"
#include "hittable_list.h"
#include "utilities.h"

using namespace raytracing;
typedef unsigned char byte;

Vector3 RayColor(const Ray3& ray, const Hittable& world, int depth)
{
	if (depth == 0)
	{
		return VECTOR3_ZERO;
	}

	HitRecord hit_record;
	if (world.Hit(ray, 0.001, 1000.0, hit_record))
	{
		Ray3 scattered;
		Vector3 attenuation;
		if (hit_record.material->Scatter(ray, hit_record, attenuation, scattered))
		{
			return attenuation * RayColor(scattered, world, depth - 1);
		}
		return VECTOR3_ZERO;
	}

	Vector3 unit_direction = ray.direction.UnitVector();
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
}

/*
void WriteColor(std::ostream& out, const Vector3& pixel_color, int samples_per_pixel)
{
	double red = std::sqrt(pixel_color.x() / samples_per_pixel);
	double green = std::sqrt(pixel_color.y() / samples_per_pixel);
	double blue = std::sqrt(pixel_color.z() / samples_per_pixel);
	// Write the translated [0,255] value of each color component.
	// TODO: Clamp is required? Why the .999?
	out << static_cast<int>(256 * Clamp(red, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(green, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(blue, 0.0, 0.999)) << '\n';
}
*/

HittableList CreateRandomScene() {
	HittableList world;

	auto ground_material = std::make_shared<LambertianMaterial>(Vector3(0.5, 0.5, 0.5));
	world.Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, ground_material));

	/*
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = GetRandomDouble();
			Vector3 center(a + 0.9 * GetRandomDouble(), 0.2, b + 0.9 * GetRandomDouble());

			if ((center - Vector3(4, 0.2, 0)).GetLength() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Vector3::GetRandom() * Vector3::GetRandom();
					sphere_material = std::make_shared<LambertianMaterial>(albedo);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = Vector3::GetRandom(0.5, 1);
					auto fuzz = GetRandomDouble(0, 0.5);
					sphere_material = std::make_shared<MetalMaterial>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<DielectricMaterial>(1.5);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	*/
	auto material1 = std::make_shared<DielectricMaterial>(1.5);
	world.Add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<LambertianMaterial>(Vector3(0.4, 0.2, 0.1));
	world.Add(std::make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<MetalMaterial>(Vector3(0.7, 0.6, 0.5), 0.0);
	world.Add(std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, material3));

	return world;
}

const int max_ray_depth = 50;
const int samples_per_pixel = 10;
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 200;
constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
constexpr int number_of_pixels = image_width * image_height;
const int number_of_threads = 1;
const int pixels_per_thread = static_cast<int>(std::ceil(number_of_pixels / number_of_threads));

void RaytraceImagePart(int thread_id, int start_idx, int end_idx, const Camera& camera, const HittableList& world, byte* image)
{
	std::cout << "Thread " << thread_id << ": Start" << std::endl;

	for (size_t i = start_idx; i <= end_idx; ++i)
	{
		int y = i / image_width;
		int x = i - y * image_width;

		Vector3 pixel_color;

		for (int s = 0; s < samples_per_pixel; ++s)
		{
			double u = (x + GetRandomDouble()) / (image_width - 1);
			double v = (y + GetRandomDouble()) / (image_height - 1);
			Ray3 ray = camera.GetRay(u, v);
			pixel_color += RayColor(ray, world, max_ray_depth);
		}

		double red = std::sqrt(pixel_color.x() / samples_per_pixel);
		double green = std::sqrt(pixel_color.y() / samples_per_pixel);
		double blue = std::sqrt(pixel_color.z() / samples_per_pixel);

		// TODO: Why 0.999?
		int byte_idx = i * 3;
		image[byte_idx] = static_cast<int>(256 * Clamp(red, 0.0, 0.999));
		image[byte_idx + 1] = static_cast<int>(256 * Clamp(blue, 0.0, 0.999));
		image[byte_idx + 2] = static_cast<int>(256 * Clamp(green, 0.0, 0.999));

		if (i % 1000 == 0)
		{
			std::cout << "Thread " << thread_id << ": " << (i - start_idx) << "/" << number_of_pixels << std::endl;
		}
	}

	std::cout << "Thread " << thread_id << ": End" << std::endl;
}

int main()
{
	HittableList world = CreateRandomScene();

	Vector3 origin(13, 2, 3);
	Vector3 look_at(0, 0, 0);
	Vector3 up(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;
	Camera camera(origin, look_at, up, 20.0, aspect_ratio, aperture, dist_to_focus);

	constexpr int number_of_bytes = number_of_pixels * 3;
	byte* image = new byte[number_of_bytes];
	if (image == nullptr)
	{
		std::cout << "Error allocating memory" << std::endl;
		return -1;
	}
	std::thread threads[number_of_threads];

	int start_idx = 0;
	for (size_t i = 0; i < number_of_threads; ++i)
	{
		int end_idx = start_idx + pixels_per_thread;
		end_idx = end_idx >= number_of_pixels ? number_of_pixels : end_idx;
		threads[i] = std::thread(RaytraceImagePart, i, start_idx, end_idx, camera, world, image);
		start_idx = end_idx + 1;
	}

	for (size_t i = 0; i < number_of_threads; ++i)
	{
		threads[i].join();
	}

	std::ofstream file;
	file.open("test.ppm");
	file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < number_of_bytes; i += 3)
	{
		file << image[i] << ' ' << image[i + 1] << ' ' << image[i + 2] << '\n';
	}

	// TODO: delete or delete[]?
	delete[] image;

	/*
	for (int y = image_height - 1; y >= 0; --y)
	{
		//std::cout << "Scanlines remaining: " << y << std::endl;
		for (int x = 0; x < image_width; ++x)
		{
			Vector3 pixel_color;

			for (int i = 0; i < samples_per_pixel; ++i)
			{
				double u = (x + GetRandomDouble()) / (image_width - 1);
				double v = (y + GetRandomDouble()) / (image_height - 1);
				Ray3 ray = camera.GetRay(u, v);
				pixel_color += RayColor(ray, world, max_ray_depth);
			}
			
			WriteColor(file, pixel_color, samples_per_pixel);
		}
	}
	*/
}
