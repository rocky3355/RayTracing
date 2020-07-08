#pragma once

#include "vector3.h"

namespace raytracing
{
class Perlin {
public:
    Perlin();
    ~Perlin();
    double Noise(const Vector3& p) const;
    double Turbulence(const Vector3& p, int depth = 7) const;

private:
    double TrilinearInterpolation(Vector3 c[2][2][2], double u, double v, double w) const;

    static int* GeneratePerm()
    {
        int* p = new int[point_count_];
        for (int i = 0; i < Perlin::point_count_; i++)
        {
            p[i] = i;
        }
        Permute(p, point_count_);
        return p;
    }

    static void Permute(int* p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = GetRandomInt(i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static const int point_count_ = 256;
    Vector3* ranvec_;
    int* perm_x_;
    int* perm_y_;
    int* perm_z_;
};
}  // namespace raytracing
