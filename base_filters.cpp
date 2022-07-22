#include "base_filters.h"
#include <cstdint>
#include <cstring>
#include <cmath>

namespace FilterHelpers {
double GaussFunction(int32_t x, int32_t y, int32_t sigma) {
    return pow(M_E, (-pow(x, 2) - pow(y, 2)) / (2 * pow(sigma, 2)))
            / (2 * M_PI * pow(sigma, 2));
}

template<typename T>
void ApplyMatrix(Bitmap& bitmap, std::vector<std::vector<T>>& matrix) {
    Bitmap::PixelArray new_pixel_array;
    new_pixel_array.SetHeight(bitmap.GetHeight());
    new_pixel_array.SetWidth(bitmap.GetWidth());
    int32_t value = matrix.size() / 2;
    for (int32_t i = 0; i < bitmap.GetHeight(); ++i) {
        for (int32_t j = 0; j < bitmap.GetWidth(); ++j) {
            Bitmap::Pixel pixel{};
            T cur_red = 0, cur_green = 0, cur_blue = 0;
            for (int32_t x_pos = -1 * value; x_pos < value + 1; ++x_pos) {
                for (int32_t y_pos = -1 * value; y_pos < value + 1; ++y_pos) {
                    int32_t x = i + x_pos;
                    int32_t y = j + y_pos;
                    x = std::min(std::max(x, 0),
                            static_cast<int32_t>(bitmap.GetHeight()) - 1);
                    y = std::min(std::max(y, 0),
                            static_cast<int32_t>(bitmap.GetWidth()) - 1);
                    cur_red += (bitmap.GetPixel(x, y).red) *
                            matrix[x_pos + value][y_pos + value];
                    cur_green += (bitmap.GetPixel(x, y).green) *
                            matrix[x_pos + value][y_pos + value];
                    cur_blue += (bitmap.GetPixel(x, y).blue) *
                            matrix[x_pos + value][y_pos + value];
                }
                pixel.red = std::min(255, std::max(0, static_cast<int32_t>(cur_red)));
                pixel.green = std::min(255, std::max(0, static_cast<int32_t>(cur_green)));
                pixel.blue = std::min(255, std::max(0, static_cast<int32_t>(cur_blue)));
            }
            new_pixel_array.PushBack(pixel);
        }
    }
    bitmap.Copy(new_pixel_array);
}
}

bool BlurFilter::Apply(Bitmap& bitmap) {
    double gauss_sum = 0;
    size_t matrix_size = sigma * 6 + 1;
    std::vector<std::vector<double>> gauss_matrix;
    for (int32_t x = -sigma * 3; x < sigma * 3 + 1; ++x) {
        std::vector<double> new_row;
        for (int32_t y = -sigma * 3; y < sigma * 3 + 1; ++y) {
            double g = FilterHelpers::GaussFunction(x, y, sigma);
            gauss_sum += g;
            new_row.push_back(g);
        }
        gauss_matrix.push_back(new_row);
    }
    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            gauss_matrix[i][j] /= gauss_sum;
        }
    }
    FilterHelpers::ApplyMatrix(bitmap, gauss_matrix);
    return true;
}

bool CropFilter::Apply(Bitmap& bitmap) {
    Bitmap::PixelArray new_pixel_array;
    new_pixel_array.SetHeight(std::min(height_, bitmap.GetHeight()));
    new_pixel_array.SetWidth(std::min(width_, bitmap.GetWidth()));
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t col = 0; col < bitmap.GetWidth(); ++col) {
            if (row >= bitmap.GetHeight() - new_pixel_array.GetHeight() && col <
            new_pixel_array.GetWidth()) {
                new_pixel_array.PushBack(bitmap.GetPixel(row, col));
            }
        }
    }
    bitmap.Copy(new_pixel_array);
    bitmap.SetDibHeight(new_pixel_array.GetHeight());
    bitmap.SetDibWidth(new_pixel_array.GetWidth());
    return true;
}

bool NegativeFilter::Apply(Bitmap& bitmap) {
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t col = 0; col < bitmap.GetWidth(); ++col) {
            bitmap.GetPixel(row, col).red = 255 - bitmap.GetPixel(row, col).red;
            bitmap.GetPixel(row, col).blue = 255 - bitmap.GetPixel(row, col).blue;
            bitmap.GetPixel(row, col).green = 255 - bitmap.GetPixel(row, col).green;
        }
    }
    return true;
}

bool GreyscaleFilter::Apply(Bitmap& bitmap) {
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t col = 0; col < bitmap.GetWidth(); ++col) {
            bitmap.GetPixel(row, col).red =
                    RedConst * bitmap.GetPixel(row, col).red +
                    GreenConst * bitmap.GetPixel(row, col).green +
                    BlueConst * bitmap.GetPixel(row, col).blue;
            bitmap.GetPixel(row, col).blue = bitmap.GetPixel(row, col).red;
            bitmap.GetPixel(row, col).green = bitmap.GetPixel(row, col).red;
        }
    }
    return true;
}

bool SharpeningFilter::Apply(Bitmap& bitmap) {
    FilterHelpers::ApplyMatrix(bitmap, matrix);
    return true;
}

bool EdgeDetectionFilter::Apply(Bitmap& bitmap) {
    GreyscaleFilter::Apply(bitmap);
    FilterHelpers::ApplyMatrix(bitmap, matrix);
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t col = 0; col < bitmap.GetWidth(); ++col) {
            if (bitmap.GetPixel(row, col).red > threshold_) {
                bitmap.GetPixel(row, col).red = 255;
                bitmap.GetPixel(row, col).green = 255;
                bitmap.GetPixel(row, col).blue = 255;
            } else {
                bitmap.GetPixel(row, col).red = 0;
                bitmap.GetPixel(row, col).green = 0;
                bitmap.GetPixel(row, col).blue = 0;
            }
        }
    }
    return true;
}

bool BlocksFilter::Apply(Bitmap& bitmap) {
    for (int32_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (int32_t col = 0; col < bitmap.GetWidth(); ++col) {
            if (row % x_ != 0 && col % y_ != 0) {
                bitmap.GetPixel(row, col).red = bitmap.GetPixel(row - (row % x_), col -
                (col % y_)).red;
                bitmap.GetPixel(row, col).blue = bitmap.GetPixel(row - (row % x_), col -
                                (col % y_)).blue;
                bitmap.GetPixel(row, col).green = bitmap.GetPixel(row - (row % x_), col -
                                (col % y_)).green;
            }
        }
    }
    return true;
}