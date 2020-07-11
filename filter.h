#pragma once

#include <stdint.h>

namespace raytracing
{
class MedianFilter
{
public:
	void Filter(const uint8_t* image_in, int image_width, int image_height, uint8_t* image_out) const;

private:
	bool IsFilteringRequired(uint8_t window[]) const;
	// TODO: Use std sort?
	void InsertionSort(uint8_t arr[], int n) const;

	// TODO: Evaluate this for sorting
	/*
	// insertion sort
    for (auto i = v.begin(); i != v.end(); ++i) {
        std::rotate(std::upper_bound(v.begin(), i, *i), i, i+1);
    }
	*/
};

class GaussianFilter
{
public:
	GaussianFilter(double sigma = 1.0);
	void Filter(const uint8_t* image_in, int image_width, int image_height, uint8_t* image_out) const;

private:
	double kernel_[3][3];
};
}  // namespace raytracing
