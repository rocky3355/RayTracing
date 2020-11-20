#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include "ray_tracer.h"
#include "scene_creator.h"
#include <CL/cl.hpp>

// TODO: How to define light strength? Mode without light?
// TODO: Create "Scene" class that contains camera, objects, lights
// TODO: size_t where applicable
// TODO: Base class for filters
// TODO: Define "Color" as Vector3?
// TODO: Rename attenuation to albedo?
// TODO: Class forward where possible
// TODO: Remove "new" where possible
// TODO: Check parameter passing, i.e. use (hit_record) instad of (hit_record.u, hit_record.v, ...)

using namespace raytracing;
using namespace std::chrono_literals;

bool rendering_finished = false;
long counter;

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

void OpenCL()
{
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

	std::vector<cl::Device> all_devices;
	all_platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	cl::Device device = all_devices[0];
	cl::Context context({ device });
	cl::Program::Sources sources;

	std::string kernel_code =
		"   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
		"       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 "
		"   }";

	sources.push_back({ kernel_code.c_str(),kernel_code.length() });

	cl::Program program(context, sources);
	if (program.build({ device }) != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		exit(1);
	}

	cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
	cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);

	int A[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };

	cl::CommandQueue queue(context, device);
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
	queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);

	cl::Kernel kernel_add = cl::Kernel(program, "simple_add");
	kernel_add.setArg(0, buffer_A);
	kernel_add.setArg(1, buffer_B);
	kernel_add.setArg(2, buffer_C);
	queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(10), cl::NullRange);
	queue.finish();

	int C[10];
	//read result C from the device to array C
	queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, C);

	std::cout << " result: \n";
	for (int i = 0; i < 10; i++) {
		std::cout << C[i] << " ";
	}
}

int main()
{
	// FinalSceneChapterTwo, 600x600, 100 rays, 50 depth, 8 threads: 100s
	//OpenCL();
	//return 0;

	RayTracingOptions options;
	options.image_width = 600;
	options.max_ray_depth = 20;
	options.number_of_threads = 8;
	options.samples_per_pixel = 500;
	options.background_color = Vector3(0.0, 0.0, 0.0);

	Camera camera;
	auto lights = new HittableList();
	SceneCreator scene_creator;
	BvhNode scene = scene_creator.FinalSceneChapterTwo(camera, lights);

	auto start = std::chrono::high_resolution_clock::now();

	// TODO: In the next run, percentage_finished will be already at 100, it will be resetet in the "Render" function
	RayTracer ray_tracer;
	std::thread print_thread([&] { PrintProgress(ray_tracer); });
	
	const uint8_t* image = ray_tracer.Render(options, camera, scene, lights);

	std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
	std::cout << "\n\nElapsed: " << elapsed.count() << "s\n";

	rendering_finished = true;
	print_thread.join();

	int image_height = static_cast<int>(options.image_width / camera.aspect_ratio);
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

	return 0;
}
