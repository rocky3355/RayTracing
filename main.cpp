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
#include "translation.h"
#include "rotation.h"
#include "constant_medium.h"
#include "pdf.h"

// TODO: Rename BHV to BVH
// TODO: define "Color" as Vector3
// TODO: Remove unnecessary empty constructors
// TODO: Rename attenuation to albedo?
// TODO: Check public/private
// TODO: Class forward where possible
// TODO: Check parameter passing, i.e. use (hit_record) instad of (hit_record.u, hit_record.v, ...)

using namespace raytracing;
using namespace std::chrono_literals;

Vector3 RayColor(const Ray3& ray, const Vector3& background, const Hittable& world, int depth)
{
	HitRecord hit_record;

	// TODO: == 0?
	if (depth <= 0)
	{
		return VECTOR3_ZERO;
	}

	if (!world.Hit(ray, 0.001, Infinity, hit_record))
	{
		return background;
	}

	double pdf_value;
	Ray3 scattered;
	Vector3 albedo;
	Vector3 emitted = hit_record.material->Emit(ray, hit_record);

	// TODO: Why is pdf_value passed here?
	if (!hit_record.material->Scatter(ray, hit_record, albedo, scattered, pdf_value))
	{
		return emitted;
	}

	std::shared_ptr<Hittable> light_shape = std::make_shared<XZRect>(213, 343, 227, 332, 554, std::shared_ptr<Material>());
	/*
	auto p0 = std::make_shared<HittablePdf>(light_shape, hit_record.point);
	auto p1 = std::make_shared<CosinePdf>(hit_record.normal);
	MixturePdf p(p0, p1);
	*/

	HittablePdf p(light_shape, hit_record.point);
	scattered = Ray3(hit_record.point, p.Generate(), ray.time);
	pdf_value = p.Value(scattered.direction);

	return emitted
			+ albedo
		    * hit_record.material->ScatterPdf(ray, hit_record, scattered)
			* RayColor(scattered, background, world, depth - 1)
		    / pdf_value;
}

HittableList CreateRandomScene()
{
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
	objects.Add(std::make_shared<FlipFace>(std::make_shared<XZRect>(213, 343, 227, 332, 554, light)));
	objects.Add(std::make_shared<FlipFace>(std::make_shared<XZRect>(0, 555, 0, 555, 0, white)));
	objects.Add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
	objects.Add(std::make_shared<FlipFace>(std::make_shared<XYRect>(0, 555, 0, 555, 555, white)));

	std::shared_ptr<Hittable> box1 = std::make_shared<Box>(VECTOR3_ZERO, Vector3(165, 330, 165), white);
	box1 = std::make_shared<RotationY>(box1, 15);
	box1 = std::make_shared<Translation>(box1, Vector3(265, 0, 295));
	objects.Add(box1);

	std::shared_ptr<Hittable> box2 = std::make_shared<Box>(VECTOR3_ZERO, Vector3(165, 165, 165), white);
	box2 = std::make_shared<RotationY>(box2, -18);
	box2 = std::make_shared<Translation>(box2, Vector3(130, 0, 65));
	objects.Add(box2);

	return objects;
}

HittableList FinalSceneChapterTwo()
{
	HittableList boxes1;
	auto ground = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; ++i)
	{
		for (int j = 0; j < boxes_per_side; ++j)
		{
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = GetRandomDouble(1, 101);
			auto z1 = z0 + w;

			boxes1.Add(std::make_shared<Box>(Vector3(x0, y0, z0), Vector3(x1, y1, z1), ground));
		}
	}

	HittableList objects;

	objects.Add(std::make_shared<BhvNode>(boxes1, 1.0));

	auto light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(7, 7, 7));
	objects.Add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

	auto center1 = Vector3(400, 400, 200);
	//auto center2 = center1 + Vector3(30, 0, 0);
	auto moving_sphere_material =
		std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(0.7, 0.3, 0.1));
	objects.Add(std::make_shared<Sphere>(center1, 50, moving_sphere_material, Vector3(30, 0, 0)));

	objects.Add(std::make_shared<Sphere>(Vector3(260, 150, 45), 50, std::make_shared<DielectricMaterial>(1.5)));
	objects.Add(std::make_shared<Sphere>(
		Vector3(0, 150, 145), 50, std::make_shared<MetalMaterial>(Vector3(0.8, 0.8, 0.9), 10.0)
		));

	auto boundary = std::make_shared<Sphere>(Vector3(360, 150, 145), 70, std::make_shared<DielectricMaterial>(1.5));
	objects.Add(boundary);
	objects.Add(std::make_shared<ConstantMedium>(
		boundary, 0.2, std::make_shared<SolidColor>(0.2, 0.4, 0.9)
		));
	boundary = std::make_shared<Sphere>(Vector3(0, 0, 0), 5000, std::make_shared<DielectricMaterial>(1.5));
	objects.Add(std::make_shared<ConstantMedium>(
		boundary, .0001, std::make_shared<SolidColor>(1, 1, 1)));

	auto emat = std::make_shared<LambertianMaterial>(std::make_shared<ImageTexture>("earthmap.jpg"));
	objects.Add(std::make_shared<Sphere>(Vector3(400, 200, 400), 100, emat));
	auto pertext = std::make_shared<NoiseTexture>(0.1);
	objects.Add(std::make_shared<Sphere>(Vector3(220, 280, 300), 80, std::make_shared<LambertianMaterial>(pertext)));

	HittableList boxes2;
	auto white = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.Add(std::make_shared<Sphere>(Vector3::GetRandom(0, 165), 10, white));
	}

	objects.Add(std::make_shared<Translation>(
		std::make_shared<RotationY>(
			std::make_shared<BhvNode>(boxes2, 1.0), 15),
		Vector3(-100, 270, 395)
		)
	);

	return objects;
}

const int max_ray_depth = 50;
const int samples_per_pixel = 100;
constexpr double aspect_ratio = 1.0 / 1.0;
constexpr int image_width = 300;
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

	HittableList world_list = CornellBox();
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
}
