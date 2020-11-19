#pragma once

#include "camera.h"
#include "hittable_list.h"

namespace raytracing
{
struct RayTracingOptions
{
	int image_width;
	int max_ray_depth;
	int samples_per_pixel;
	int number_of_threads;
	Vector3 background_color;
};

struct RayTracingOptionsInternal
{
	RayTracingOptions options;
	int thread_start_idx;
	int thread_end_idx;
	int image_width_extended;
	int image_height_extended;
	int number_of_pixels;
};

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	const uint8_t* Render(const RayTracingOptions& options, const Camera& camera, const Hittable& scene, Hittable* lights);

	int percentage_finished;

private:
	Vector3 RayColor(const Ray3& ray, const Vector3& background, const Hittable& scene, Hittable* lights, int depth) const;
	void RenderImagePart(RayTracingOptionsInternal options, const Camera& camera, const Hittable& scene, Hittable* lights);

	uint8_t* image_;
	uint8_t* filtered_image_;
	int number_of_rendered_pixels_;
};
}  // namespace raytracing
