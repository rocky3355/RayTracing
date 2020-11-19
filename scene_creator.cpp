#include "scene_creator.h"

#include "all_hittables.h"
#include "all_materials.h"
#include "all_textures.h"

namespace raytracing
{
BvhNode SceneCreator::CreateRandomScene(Camera& camera, HittableList* lights) const
{
	HittableList world;

	CheckerTexture* checker_texture = new CheckerTexture(
		new SolidColor(Vector3(0.2, 0.3, 0.1)),
		new SolidColor(Vector3(0.9, 0.9, 0.9)));

	auto ground_material = new LambertianMaterial(checker_texture);
	world.Add(new Sphere(Vector3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = GetRandomDouble();
			Vector3 center(a + 0.9 * GetRandomDouble(), 0.2, b + 0.9 * GetRandomDouble());

			if ((center - Vector3(4, 0.2, 0)).GetLength() > 0.9) {
				Material* sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Vector3::GetRandom() * Vector3::GetRandom();
					sphere_material = new LambertianMaterial(new SolidColor(albedo));
					world.Add(new Sphere(center, 0.2, sphere_material, Vector3(0.0, 0.0, 0.0)));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = Vector3::GetRandom(0.5, 1);
					auto fuzz = GetRandomDouble(0, 0.5);
					sphere_material = new MetalMaterial(albedo, fuzz);
					world.Add(new Sphere(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = new DielectricMaterial(1.5);
					world.Add(new Sphere(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = new DielectricMaterial(1.5);
	world.Add(new Sphere(Vector3(0, 1, 0), 1.0, material1));

	auto material2 = new LambertianMaterial(new SolidColor(Vector3(0.4, 0.2, 0.1)));
	world.Add(new Sphere(Vector3(-4, 1, 0), 1.0, material2));

	auto material3 = new MetalMaterial(Vector3(0.7, 0.6, 0.5), 0.1);
	world.Add(new Sphere(Vector3(4, 1, 0), 1.0, material3));

	//lights->Add(new Sphere(Vector3(4, 1, 0), 1.0, Material*()));
	//lights->Add(new GlobalLight(Vector3(1.0, 1.0, 1.0)));

	Vector3 origin(13, 3, 7);
	Vector3 look_at(0, 1, 0);
	Vector3 up(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;
	double vfov = 20.0;
	double aspect = 16.0 / 9.0;
	double time_end = 1.0;

	camera = Camera(origin, look_at, up, vfov, aspect, aperture, dist_to_focus, time_end);
	return BvhNode(world, time_end);
}

HittableList SceneCreator::CornellBox(Camera& camera, HittableList* lights) const
{
	HittableList world;

	auto red = new LambertianMaterial(new SolidColor(.65, .05, .05));
	auto white = new LambertianMaterial(new SolidColor(.73, .73, .73));
	auto green = new LambertianMaterial(new SolidColor(.12, .45, .15));
	auto light = new DiffuseLight(new SolidColor(15, 15, 15));

	world.Add(new FlipFace(new YZRect(0, 555, 0, 555, 555, green)));
	world.Add(new YZRect(0, 555, 0, 555, 0, red));
	world.Add(new FlipFace(new XZRect(213, 343, 227, 332, 554, light)));
	world.Add(new FlipFace(new XZRect(0, 555, 0, 555, 555, white)));
	world.Add(new XZRect(0, 555, 0, 555, 0, white));
	world.Add(new FlipFace(new XYRect(0, 555, 0, 555, 555, white)));

	Hittable* box1 = new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white);
	box1 = new RotationY(box1, 15);
	box1 = new Translation(box1, Vector3(265, 0, 295));
	world.Add(box1);

	/*
	Hittable* box2 = new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white);
	box2 = new RotationY(box2, -18);
	box2 = new Translation(box2, Vector3(130, 0, 65));
	world.Add(box2);
	*/

	auto glass = new DielectricMaterial(1.5);
	world.Add(new Sphere(Vector3(190, 90, 190), 90, glass));

	lights->Add(new XZRect(213, 343, 227, 332, 554, new Material()));
	lights->Add(new Sphere(Vector3(190, 90, 190), 90, new Material()));

	Vector3 origin(278, 278, -800);
	Vector3 look_at(278, 278, 0);
	Vector3 up(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.0;
	double vfov = 40.0;
	double aspect = 1.0;
	double t_end = 1.0;

	camera = Camera(origin, look_at, up, vfov, aspect, aperture, dist_to_focus, t_end);
	return world;
}

BvhNode SceneCreator::FinalSceneChapterTwo(Camera& camera, HittableList* lights) const
{
	HittableList boxes1;
	auto ground = new LambertianMaterial(new SolidColor(0.48, 0.83, 0.53));

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

			boxes1.Add(new Box(Vector3(x0, y0, z0), Vector3(x1, y1, z1), ground));
		}
	}

	HittableList objects;
	objects.Add(new BvhNode(boxes1, 1.0));

	auto light = new DiffuseLight(new SolidColor(7, 7, 7));
	objects.Add(new FlipFace(new XZRect(123, 423, 147, 412, 554, light)));
	lights->Add(new XZRect(123, 423, 147, 412, 554, new Material()));

	auto center1 = Vector3(400, 400, 200);
	//auto center2 = center1 + Vector3(30, 0, 0);
	auto moving_sphere_material =
		new LambertianMaterial(new SolidColor(0.7, 0.3, 0.1));
	objects.Add(new Sphere(center1, 50, moving_sphere_material, Vector3(30, 0, 0)));

	objects.Add(new Sphere(Vector3(260, 150, 45), 50, new DielectricMaterial(1.5)));
	objects.Add(new Sphere(
		Vector3(0, 150, 145), 50, new MetalMaterial(Vector3(0.8, 0.8, 0.9), 0.0)
		));

	auto boundary = new Sphere(Vector3(360, 150, 145), 70, new DielectricMaterial(1.5));
	objects.Add(boundary);
	objects.Add(new ConstantMedium(
		boundary, 0.2, new SolidColor(0.2, 0.4, 0.9)
		));
	boundary = new Sphere(Vector3(0, 0, 0), 5000, new DielectricMaterial(1.5));
	objects.Add(new ConstantMedium(
		boundary, .0001, new SolidColor(1, 1, 1)));

	auto emat = new LambertianMaterial(new ImageTexture("earthmap.jpg"));
	objects.Add(new Sphere(Vector3(400, 200, 400), 100, emat));
	auto pertext = new NoiseTexture(0.1);
	objects.Add(new Sphere(Vector3(220, 280, 300), 80, new LambertianMaterial(pertext)));

	HittableList boxes2;
	auto white = new LambertianMaterial(new SolidColor(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.Add(new Sphere(Vector3::GetRandom(0, 165), 10, white));
	}

	objects.Add(new Translation(
		new RotationY(
			new BvhNode(boxes2, 1.0), 15),
		Vector3(-100, 270, 395)
		)
	);

	Vector3 origin(478, 278, -600);
	Vector3 look_at(278, 278, 0);
	Vector3 up(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.0;
	double vfov = 40.0;
	double aspect = 1.0;
	double time_end = 1.0;

	camera = Camera(origin, look_at, up, vfov, aspect, aperture, dist_to_focus, time_end);
	return BvhNode(objects, time_end);
}
}  // namespace raytracing
