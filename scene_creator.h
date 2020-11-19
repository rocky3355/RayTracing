#pragma once

#include "bvh_node.h"
#include "camera.h"
#include "hittable_list.h"

namespace raytracing
{
class SceneCreator
{
public:
	BvhNode CreateRandomScene(Camera& camera, HittableList* lights) const;
	HittableList CornellBox(Camera& camera, HittableList* lights) const;
	BvhNode FinalSceneChapterTwo(Camera& camera, HittableList* lights) const;
};
}  // namespace raytracing
