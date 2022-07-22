#pragma once

#include "bitmap.h"

class BaseFilter {
public:
    virtual bool Apply(Bitmap& bitmap) = 0;
    virtual ~BaseFilter() = default;
};

class BlurFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
    int32_t sigma = 0;
};

class CropFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
    int32_t height_;
    int32_t width_;
};

class NegativeFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
};

class GreyscaleFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
    constexpr static const double RedConst = 0.299;
    constexpr static const double GreenConst = 0.587;
    constexpr static const double BlueConst = 0.114;
};

class SharpeningFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
protected:
    std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
};

class EdgeDetectionFilter : public GreyscaleFilter {
public:
    bool Apply(Bitmap& bitmap) override;
    int32_t threshold_ = 0;
protected:
    std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
};

class BlocksFilter : public BaseFilter {
public:
    bool Apply(Bitmap& bitmap) override;
    int32_t x_ = 1;
    int32_t y_ = 1;
};