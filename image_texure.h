#pragma once

#include "texture.h"

namespace raytracing
{
class ImageTexture : public Texture 
{
public:
    ImageTexture(const char* filename);
    ~ImageTexture();
    virtual Vector3 GetColor(double u, double v, const Vector3& p) const;

private:
	const static int bytes_per_pixel_ = 3;
    unsigned char* data_;
    int width_;
    int height_;
    int bytes_per_scanline_;
};
}  // namespace raytracing
