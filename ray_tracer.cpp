#include <thread>
#include "filter.h"
#include "material.h"
#include "ray_tracer.h"

namespace raytracing
{
RayTracer::RayTracer()
	: image_(nullptr), filtered_image_(nullptr), percentage_finished(0), number_of_rendered_pixels_(0)
{
}

RayTracer::~RayTracer()
{
	// TODO: delete or delete[]?
	if (image_ != nullptr)
	{
		delete[] image_;
	}

	if (filtered_image_ != nullptr)
	{
		delete[] filtered_image_;
	}
}

const uint8_t* RayTracer::Render(const RayTracingOptions& options, const Camera& camera, const Hittable& scene, Hittable* lights)
{
	percentage_finished = 0;
	number_of_rendered_pixels_ = 0;

	int image_height = static_cast<int>(options.image_width / camera.aspect_ratio);
	// +2 for the Median filter applied afterwards
	int image_width_extended = options.image_width + 2;
	int image_height_extended = image_height + 2;
	int number_of_pixels = options.image_width * image_height;
	int number_of_pixels_extended = image_width_extended * image_height_extended;
	int pixels_per_thread = static_cast<int>(std::ceil((double)number_of_pixels_extended / options.number_of_threads));
	int number_of_bytes = number_of_pixels * 3;
	int number_of_bytes_extended = number_of_pixels_extended * 3;

	image_ = new uint8_t[number_of_bytes_extended];
	if (image_ == nullptr)
	{
		std::cout << "Error allocating memory" << std::endl;
		return nullptr;
	}

	std::vector<std::thread> render_threads;

	for (std::size_t i = 0U; i < options.number_of_threads; ++i)
	{
		RayTracingOptionsInternal internal_options;
		internal_options.options = options;
		internal_options.image_width_extended = image_width_extended;
		internal_options.image_height_extended = image_height_extended;
		internal_options.thread_start_idx = i;
		internal_options.number_of_threads = options.number_of_threads;
		internal_options.number_of_pixels = number_of_pixels_extended;

		// TODO: Correct capture?
		render_threads.emplace_back([this, internal_options, &camera, &scene, &lights] {
			this->RenderImagePart(internal_options, camera, scene, lights); });
	}

	for (std::size_t i = 0U; i < render_threads.size(); ++i)
	{
		render_threads[i].join();
	}

	MedianFilter filter;
	filtered_image_ = new uint8_t[number_of_bytes];
	filter.Filter(image_, image_width_extended, image_height_extended, filtered_image_);

	delete[] image_;
	image_ = nullptr;

	return filtered_image_;
}

void RayTracer::RenderImagePart(RayTracingOptionsInternal options, const Camera& camera, const Hittable& scene, Hittable* lights)
{
	for (std::size_t i = options.thread_start_idx; i < options.number_of_pixels; i += options.number_of_threads)
	{
		int y = options.image_height_extended - i / options.image_width_extended - 1;
		int x = i % options.image_width_extended;

		Vector3 pixel_color;

		for (int s = 0; s < options.options.samples_per_pixel; ++s)
		{
			double u = (x + GetRandomDouble()) / (options.image_width_extended - 1);
			double v = (y + GetRandomDouble()) / (options.image_width_extended - 1);
			Ray3 ray = camera.GetRay(u, v);
			pixel_color += RayColor(ray, options.options.background_color, scene, lights, options.options.max_ray_depth);
		}

		double red = pixel_color.x();
		double green = pixel_color.y();
		double blue = pixel_color.z();

		// Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
		red = std::isnan(red) ? 0.0 : red;
		green = std::isnan(green) ? 0.0 : green;
		blue = std::isnan(blue) ? 0.0 : blue;

		red = std::sqrt(red / options.options.samples_per_pixel);
		green = std::sqrt(green / options.options.samples_per_pixel);
		blue = std::sqrt(blue / options.options.samples_per_pixel);

		// TODO: Why 0.999? Is Clamp really required?
		int byte_idx = i * 3;
		image_[byte_idx] = static_cast<uint8_t>(255 * Clamp(red, 0.0, 1.0));
		image_[byte_idx + 1] = static_cast<uint8_t>(255 * Clamp(green, 0.0, 1.0));
		image_[byte_idx + 2] = static_cast<uint8_t>(255 * Clamp(blue, 0.0, 1.0));

		number_of_rendered_pixels_++;
		percentage_finished = static_cast<int>(std::round(100.0 * number_of_rendered_pixels_ / options.number_of_pixels));
	}
}

Vector3 RayTracer::RayColor(const Ray3& ray, const Vector3& background, const Hittable& scene, Hittable* lights, int depth) const
{
	HitRecord hit_record;

	if (depth == 0)
	{
		return VECTOR3_ZERO;
	}

	// If the ray hits nothing, return the background color.
	if (!scene.Hit(ray, 0.001, Infinity, hit_record))
	{
		return background;
	}

	Ray3 scattered_ray;
	Vector3 attenuation;
	Vector3 emitted = hit_record.material->Emit(hit_record);

	if (!hit_record.material->Scatter(ray, hit_record, attenuation, scattered_ray))
	{
		// Only light sources don't scatter
		return emitted;
	}

	return emitted + attenuation * RayColor(scattered_ray, background, scene, lights, depth - 1);
}
}  // namespace raytracing
