#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include "ray_tracer.h"
#include "scene_creator.h"
#include <CL/cl.hpp>

// TODO: Create "Scene" class that contains camera, objects, lights
// TODO: size_t where applicable
// TODO: base class for filters
// TODO: define "Color" as Vector3?
// TODO: Rename attenuation to albedo?
// TODO: Class forward where possible
// TODO: Check parameter passing, i.e. use (hit_record) instad of (hit_record.u, hit_record.v, ...)

using namespace raytracing;
using namespace std::chrono_literals;

bool rendering_finished = false;

void PrintProgress(const RayTracer& ray_tracer)
{
	int last_percentage = 0;
	while (!rendering_finished)
	{
		int percentage = ray_tracer.percentage_finished;
		if (percentage > last_percentage)
		{
			std::cout << percentage << "%, ";
			last_percentage = percentage;
		}
		std::this_thread::sleep_for(100ms);
	}

	std::cout << std::endl;
}

/*
//get all platforms (drivers)
std::vector<cl::Platform> all_platforms;
cl::Platform::get(&all_platforms);
if (all_platforms.size() == 0)
{
	std::cout << " No platforms found. Check OpenCL installation!\n";
	exit(1);
}

for (const cl::Platform& platform : all_platforms)
{
	std::cout << "Platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";

	std::vector<cl::Device> all_devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0)
	{
		std::cout << "   No devices found. Check OpenCL installation!\n";
		exit(1);
	}
	for (const cl::Device& device : all_devices)
	{
		std::cout << "    - " << device.getInfo<CL_DEVICE_NAME>() << ", Version: " << device.getInfo<CL_DEVICE_VERSION>() << "\n";
	}
}
*/

int main()
{
	// FinalSceneChapterTwo, 600x600, 100 rays, 50 depth: 100s

	RayTracingOptions options;
	options.aspect_ratio = 1.0;
	options.image_width = 600;
	options.max_ray_depth = 50;
	options.number_of_threads = 8;
	options.samples_per_pixel = 100;

	Camera camera;
	auto lights = std::make_shared<HittableList>();
	SceneCreator scene_creator;
	BvhNode scene = scene_creator.FinalSceneChapterTwo(camera, options.aspect_ratio, lights);

	auto start = std::chrono::high_resolution_clock::now();

	// TODO: In the next run, percentage_finished will be already at 100, it will be resetet in the "Render" function
	RayTracer ray_tracer;
	std::thread print_thread([&] { PrintProgress(ray_tracer); });
	
	const uint8_t* image = ray_tracer.Render(options, camera, scene, lights);

	std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << "\n\nElapsed: " << elapsed.count() << "s\n";

	rendering_finished = true;
	print_thread.join();

	int image_height = static_cast<int>(options.image_width / options.aspect_ratio);
	int number_of_bytes = options.image_width * image_height * 3;

	std::ofstream file;
	file.open("test.ppm");
	// TODO: Use P6 format
	file << "P3\n" << options.image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < number_of_bytes; i += 3)
	{
		file << std::to_string(image[i]) << ' ' << std::to_string(image[i + 1]) << ' ' << std::to_string(image[i + 2]) << '\n';
	}
	//file.write(reinterpret_cast<char*>(image), number_of_bytes);
}
