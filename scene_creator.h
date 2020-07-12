#pragma once

#include "bvh_node.h"
#include "camera.h"
#include "hittable_list.h"

namespace raytracing
{
class SceneCreator
{
public:
	BvhNode CreateRandomScene(Camera& camera, std::shared_ptr<HittableList> lights) const;
	HittableList CornellBox(Camera& camera, std::shared_ptr<HittableList> lights) const;
	BvhNode FinalSceneChapterTwo(Camera& camera, std::shared_ptr<HittableList> lights) const;
};
}  // namespace raytracing
