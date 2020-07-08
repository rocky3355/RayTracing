#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include "bhv_node.h"
#include "sphere.h"
#include "camera.h"
#include "diffuse_light.h"
#include "checker_texture.h"
#include "noise_texture.h"
#include "image_texure.h"
#include "solid_color.h"
#include "lambertian_material.h"
#include "dielectric_material.h"
#include "metal_material.h"
#include "utilities.h"
#include "aa_rect.h"
#include "flip_face.h"
#include "box.h"

using namespace raytracing;
using namespace std::chrono_literals;

// TODO: Remove unnecessary empty constructors

Vector3 RayColor(const Ray3& ray, const Vector3& background, const Hittable& world, int depth)
{
	HitRecord hit_record;

	// TODO: == 0?
	if (depth <= 0)
	{
		return VECTOR3_ZERO;
	}

	// TODO: Infinity instead of 10000
	if (!world.Hit(ray, 0.001, 100000.0, hit_record))
	{
		return background;
	}

	Ray3 scattered;
	Vector3 attenuation;
	Vector3 emitted = hit_record.material->Emit(hit_record.u, hit_record.v, hit_record.point);

	if (!hit_record.material->Scatter(ray, hit_record, attenuation, scattered))
	{
		return emitted;
	}

	return emitted + attenuation * RayColor(scattered, background, world, depth - 1);
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

	std::shared_ptr<CheckerTexture> checker_texture = std::make_shared<CheckerTexture>(
		std::make_shared<SolidColor>(Vector3(0.2, 0.3, 0.1)),
		std::make_shared<SolidColor>(Vector3(0.9, 0.9, 0.9)));

	auto ground_material = std::make_shared<LambertianMaterial>(checker_texture);
	world.Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, ground_material));
	
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = GetRandomDouble();
			Vector3 center(a + 0.9 * GetRandomDouble(), 0.2, b + 0.9 * GetRandomDouble());

			if ((center - Vector3(4, 0.2, 0)).GetLength() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Vector3::GetRandom() * Vector3::GetRandom();
					sphere_material = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(albedo));
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material, Vector3(0.0, 0.0, 0.0)));
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

	auto material1 = std::make_shared<DielectricMaterial>(1.5);
	world.Add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, material1));
	
	auto material2 = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(Vector3(0.4, 0.2, 0.1)));
	world.Add(std::make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, material2));
	
	
	auto material3 = std::make_shared<MetalMaterial>(Vector3(0.7, 0.6, 0.5), 0.0);
	world.Add(std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, material3));

	return world;
}

HittableList TwoPerlinSpheres()
{
	HittableList objects;
	auto pertext = std::make_shared<NoiseTexture>(5.0);
	objects.Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, std::make_shared<LambertianMaterial>(pertext)));
	objects.Add(std::make_shared<Sphere>(Vector3(0, 2, 0), 2, std::make_shared<LambertianMaterial>(pertext)));
	return objects;
}

HittableList Earth()
{
	auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
	auto earth_surface = std::make_shared<LambertianMaterial>(earth_texture);
	auto globe = std::make_shared<Sphere>(VECTOR3_ZERO, 2.0, earth_surface);
	return HittableList(globe);
}

HittableList SimpleLight()
{
	HittableList objects;

	auto pertext = std::make_shared<NoiseTexture>(4.0);
	objects.Add(std::make_shared<Sphere>(Vector3(0, -1000, 0), 1000, std::make_shared<LambertianMaterial>(pertext)));
	objects.Add(std::make_shared<Sphere>(Vector3(0, 2, 0), 2, std::make_shared<LambertianMaterial>(pertext)));

	auto difflight = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(4, 4, 4));
	objects.Add(std::make_shared<Sphere>(Vector3(0, 7, 0), 2, difflight));
	objects.Add(std::make_shared<XYRect>(3.0, 5.0, 1.0, 3.0, -2.0, difflight));

	return objects;
}

HittableList CornellBox()
{
	HittableList objects;

	auto red = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(.65, .05, .05));
	auto white = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(.73, .73, .73));
	auto green = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(15, 15, 15));

	objects.Add(std::make_shared<FlipFace>(std::make_shared<YZRect>(0, 555, 0, 555, 555, green)));
	objects.Add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
	objects.Add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
	objects.Add(std::make_shared<FlipFace>(std::make_shared<XZRect>(0, 555, 0, 555, 0, white)));
	objects.Add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
	objects.Add(std::make_shared<FlipFace>(std::make_shared<XYRect>(0, 555, 0, 555, 555, white)));

	objects.Add(std::make_shared<Box>(Vector3(130, 0, 65), Vector3(295, 165, 230), white));
	objects.Add(std::make_shared<Box>(Vector3(265, 0, 295), Vector3(430, 330, 460), white));

	return objects;
}

const int max_ray_depth = 50;
const int samples_per_pixel = 50;
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 800;
constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
constexpr int number_of_pixels = image_width * image_height;
const int number_of_threads = 8;
const int pixels_per_thread = static_cast<int>(std::ceil((double)number_of_pixels / number_of_threads));
int number_of_rendered_pixels = 0;
bool rendering_finished = false;

void RaytraceImagePart(int start_idx, int end_idx, const Camera& camera, const BhvNode& world, uint8_t* image)
{
	for (size_t i = start_idx; i <= end_idx; ++i)
	{
		int y = image_height - i / image_width - 1;
		int x = i % image_width;

		Vector3 pixel_color;

		for (int s = 0; s < samples_per_pixel; ++s)
		{
			double u = (x + GetRandomDouble()) / (image_width - 1);
			double v = (y + GetRandomDouble()) / (image_height - 1);
			Ray3 ray = camera.GetRay(u, v);
			pixel_color += RayColor(ray, VECTOR3_ZERO, world, max_ray_depth);
		}

		double red = std::sqrt(pixel_color.x() / samples_per_pixel);
		double green = std::sqrt(pixel_color.y() / samples_per_pixel);
		double blue = std::sqrt(pixel_color.z() / samples_per_pixel);

		// TODO: Why 0.999? Is Clamp really required?
		int byte_idx = i * 3;
		image[byte_idx] = static_cast<uint8_t>(255 * Clamp(red, 0.0, 1.0));
		image[byte_idx + 1] = static_cast<uint8_t>(255 * Clamp(green, 0.0, 1.0));
		image[byte_idx + 2] = static_cast<uint8_t>(255 * Clamp(blue, 0.0, 1.0));

		number_of_rendered_pixels++;
	}
}

void PrintProgress()
{
	int last_percentage = 0;
	while (!rendering_finished)
	{
		int percentage = static_cast<int>(std::round((double)number_of_rendered_pixels / number_of_pixels * 100.0));
		if (percentage > last_percentage)
		{
			std::cout << percentage << "%, ";
			last_percentage = percentage;
		}
		std::this_thread::sleep_for(100ms);
	}

	std::cout << std::endl;
}

int main()
{
	double time_end = 1.0;
	Vector3 origin(278, 278, -800);
	Vector3 look_at(278, 278, 0);
	Vector3 up(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.0;
	double vertical_fov = 40.0;
	Camera camera(origin, look_at, up, vertical_fov, aspect_ratio, aperture, dist_to_focus, time_end);

	HittableList world_list = SimpleLight();
	BhvNode world = BhvNode(world_list, time_end);

	constexpr int number_of_bytes = number_of_pixels * 3;
	uint8_t* image = new uint8_t[number_of_bytes];
	if (image == nullptr)
	{
		std::cout << "Error allocating memory" << std::endl;
		return -1;
	}

	int start_idx = 0;
	std::thread threads[number_of_threads];

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < number_of_threads; ++i)
	{
		int end_idx = start_idx + pixels_per_thread;
		end_idx = end_idx >= number_of_pixels ? (number_of_pixels - 1) : end_idx;
		threads[i] = std::thread(RaytraceImagePart, start_idx, end_idx, camera, world, image);
		start_idx = end_idx + 1;
	}

	std::thread print_thread(PrintProgress);

	for (size_t i = 0; i < number_of_threads; ++i)
	{
		threads[i].join();
	}

	std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << "\n\nElapsed: " << elapsed.count() << "s\n";

	// 197s, 35s with BhvNode

	rendering_finished = true;
	// TODO: Check for joinable?
	print_thread.join();

	std::ofstream file;
	file.open("test.ppm");
	// TODO: Use P6 format
	file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < number_of_bytes; i += 3)
	{
		file << std::to_string(image[i]) << ' ' << std::to_string(image[i + 1]) << ' ' << std::to_string(image[i + 2]) << '\n';
	}
	//file.write(reinterpret_cast<char*>(image), number_of_bytes);

	// TODO: delete or delete[]?
	delete[] image;

	/*
	for (int y = image_height - 1; y >= 0; --y)
	{
		std::cout << "Scanlines remaining: " << y << std::endl;
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
