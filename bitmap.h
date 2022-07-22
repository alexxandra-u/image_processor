#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

class Bitmap{
public:
    struct BMPHeader{
        uint16_t signature;
        uint32_t file_size;
        uint32_t dummy;
        uint32_t bitarray;
    } __attribute__((__packed__));

    struct DIBHeader {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint64_t dummy;
        uint64_t dummy2;
        uint64_t dummy3;
        uint32_t dummy4;
    } __attribute__((__packed__));

    struct Pixel {
        uint8_t blue = 0;
        uint8_t green = 0;
        uint8_t red = 0;
    } __attribute__((__packed__));

public:
    
    void Load(std::istream& stream);
    bool Load(const char* file_name);
    bool WriteFile(const char* file_name);
    bool WriteFile(std::ostream& stream);

public:
    void SetHeight(size_t row) {
        pixel_array_.SetHeight(row);
    }

    void SetWidth(size_t col) {
        pixel_array_.SetWidth(col);
    }

    const Pixel& GetPixel(size_t row, size_t col) const {
        return pixel_array_.GetPixel(row, col);
    }

    Pixel& GetPixel(size_t row, size_t col) {
        return pixel_array_.GetPixel(row, col);
    }

    void PushBack(Pixel pixel) {
        pixel_array_.PushBack(pixel);
    }

    int32_t GetHeight() const {
        return pixel_array_.GetHeight();
    }

    int32_t GetWidth() const {
        return pixel_array_.GetWidth();
    }

    bool IsEmpty() const {
        return pixel_array_.IsEmpty();
    }

    void SetDibWidth(int32_t width) {
        dib_header_.width = width;
    }

    void SetDibHeight(int32_t height) {
        dib_header_.height = height;
    }

public:
    class PixelArray {
    public:
        PixelArray() : height_(0), width_(0) {};

        void SetHeight(size_t row) {
            height_ = row;
        }

        void SetWidth(size_t col) {
            width_ = col;
        }

        const Pixel& GetPixel(size_t row, size_t col) const {
            return data_[width_ * row + col];
        }

        Pixel& GetPixel(size_t row, size_t col) {
            return data_[width_ * row + col];
        }

        void PushBack(Pixel pixel) {
            data_.push_back(pixel);
        }

        int32_t GetHeight() const {
            return height_;
        }

        int32_t GetWidth() const {
            return width_;
        }

        bool IsEmpty() const {
            return data_.empty();
        }

        void Clear() {
            data_.clear();
        }

    protected:
        std::vector<Pixel> data_;
        int32_t height_;
        int32_t width_;
    };

public:
    void Copy(const PixelArray& other) {
        pixel_array_ = other;
    }

protected:
    PixelArray pixel_array_;
    BMPHeader bmp_header_;
    DIBHeader dib_header_;
};