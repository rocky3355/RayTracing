#pragma once

#include "bvh_node.h"
#include "camera.h"
#include "hittable_list.h"

namespace raytracing
{
class SceneCreator
{
public:
	//HittableList CreateRandomScene() const;
	HittableList CornellBox(Camera& camera, double aspect, std::shared_ptr<HittableList> lights) const;
	BvhNode FinalSceneChapterTwo(Camera& camera, double aspect, std::shared_ptr<HittableList> lights) const;
};
}  // namespace raytracing
