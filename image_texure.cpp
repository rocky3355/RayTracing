#include "image_texure.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace raytracing
{
ImageTexture::ImageTexture(const char* file_name)
{
    int components_per_pixel = bytes_per_pixel_;
    data_ = stbi_load(file_name, &width_, &height_, &components_per_pixel, components_per_pixel);

    if (!data_)
    {
        std::cerr << "ERROR: Could not load texture image file '" << file_name << "'.\n";
        width_ = height_ = 0;
    }

    bytes_per_scanline_ = bytes_per_pixel_ * width_;
}

ImageTexture::~ImageTexture()
{
    delete data_;
}

Vector3 ImageTexture::GetColor(double u, double v, const Vector3& p) const
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data_ == nullptr)
    {
        return Vector3(0.0, 1.0, 1.0);
    }

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = Clamp(u, 0.0, 1.0);
    v = 1.0 - Clamp(v, 0.0, 1.0);  // Flip V to image coordinates

    int i = static_cast<int>(u * width_);
    int j = static_cast<int>(v * height_);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width_)
    {
        i = width_ - 1;
    }
    if (j >= height_)
    {
        j = height_ - 1;
    }

    const double color_scale = 1.0 / 255.0;
    auto pixel = data_ + j * bytes_per_scanline_ + i * bytes_per_pixel_;

    return Vector3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}
}  // namespace raytracing
