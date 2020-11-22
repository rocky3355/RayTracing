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
	BvhNode CornellBox(Camera& camera, HittableList* lights) const;
	BvhNode FinalSceneChapterTwo(Camera& camera, HittableList* lights) const;
	BvhNode CreateSimpleScene(Camera& camera, HittableList* lights) const;
};
}  // namespace raytracing
