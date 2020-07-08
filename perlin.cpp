#include "perlin.h"

namespace raytracing
{
Perlin::Perlin()
{
    ranvec_ = new Vector3[point_count_];
    for (int i = 0; i < point_count_; ++i)
    {
        ranvec_[i] = Vector3::GetRandom(-1.0, 1.0).UnitVector();
    }

    perm_x_ = GeneratePerm();
    perm_y_ = GeneratePerm();
    perm_z_ = GeneratePerm();
}

Perlin::~Perlin() {
    delete[] ranvec_;
    delete[] perm_x_;
    delete[] perm_y_;
    delete[] perm_z_;
}

double Perlin::Noise(const Vector3& p) const
{
    double u = p.x() - std::floor(p.x());
    double v = p.y() - std::floor(p.y());
    double w = p.z() - std::floor(p.z());
    int i = std::floor(p.x());
    int j = std::floor(p.y());
    int k = std::floor(p.z());
    Vector3 c[2][2][2];

    for (int di = 0; di < 2; ++di)
    {
        for (int dj = 0; dj < 2; ++dj)
        {
            for (int dk = 0; dk < 2; ++dk)
            {
                c[di][dj][dk] = ranvec_[
                    perm_x_[(i + di) & 255] ^
                    perm_y_[(j + dj) & 255] ^
                    perm_z_[(k + dk) & 255]
                ];
            }
        }
    }

    return TrilinearInterpolation(c, u, v, w);
}

double Perlin::Turbulence(const Vector3& p, int depth) const
{
    double accum = 0.0;
    Vector3 temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; ++i)
    {
        accum += weight * Noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return std::fabs(accum);
}

double Perlin::TrilinearInterpolation(Vector3 c[2][2][2], double u, double v, double w) const
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0.0;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                Vector3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww))
                    * c[i][j][k].Dot(weight_v);
            }
        }
    }

    return accum;
}
}  // namespace raytracing
