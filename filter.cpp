#include "filter.h"
#include <cmath>

namespace raytracing
{
void MedianFilter::Filter(const uint8_t* image_in, int image_width, int image_height, uint8_t* image_out) const
{
    uint8_t window[9];
    int out_idx = 0;

	for (int y = 1; y < image_height - 1; ++y)
	{
		for (int x = 1; x < image_width - 1; ++x)
		{
            int idx = (y * image_width + x) * 3;

            for (int i = 0; i < 3; ++i)
            {
                window[0] = image_in[idx + i];
                window[1] = image_in[idx - 3 + i];
                window[2] = image_in[idx + 3 + i];
                window[3] = image_in[idx - image_width * 3 + i];
                window[4] = image_in[idx + image_width * 3 + i];
                window[5] = image_in[idx - (image_width + 1) * 3 + i];
                window[6] = image_in[idx - (image_width - 1) * 3 + i];
                window[7] = image_in[idx + (image_width - 1) * 3 + i];
                window[8] = image_in[idx + (image_width + 1) * 3 + i];

                if (IsFilteringRequired(window))
                {
                    InsertionSort(window, 9);
                    image_out[out_idx++] = window[4];
                }
                else
                {
                    image_out[out_idx++] = image_in[idx + i];
                }
            }
		}
	}
}

bool MedianFilter::IsFilteringRequired(uint8_t window[]) const
{
    int outlier_counter = 0;
    uint8_t center_pixel = window[0];

    for (int i = 1; i < 9; ++i)
    {
        if (std::abs(window[i] - center_pixel) > 20)
        {
            outlier_counter++;
        }
    }

    return outlier_counter == 8;
}

void MedianFilter::InsertionSort(uint8_t arr[], int n) const
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

GaussianFilter::GaussianFilter(double sigma)
{
    double sum = 0.0;
    double s = 2.0 * sigma * sigma;
    
    // generating 3x3 kernel 
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            double r = std::sqrt(x * x + y * y);
            kernel_[x + 1][y + 1] = (std::exp(-(r * r) / s)) / (M_PI * s);
            sum += kernel_[x + 1][y + 1];
        }
    }

    // normalising the Kernel 
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            kernel_[i][j] /= sum;
        }
    }
}

void GaussianFilter::Filter(const uint8_t* image_in, int image_width, int image_height, uint8_t* image_out) const
{
    uint8_t window[9];
    int out_idx = 0;

    for (int y = 1; y < image_height - 1; ++y)
    {
        for (int x = 1; x < image_width - 1; ++x)
        {
            int idx = (y * image_width + x) * 3;

            for (int i = 0; i < 3; ++i)
            {
                uint8_t result = 0;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -1; b <= 1; ++b)
                    {
                        result += static_cast<uint8_t>(kernel_[a + 1][b + 1] * image_in[idx + i + (a + b * image_width) * 3]);
                    }
                }
                image_out[out_idx++] = result;
            }
        }
    }
}
}  // namespace raytracing
