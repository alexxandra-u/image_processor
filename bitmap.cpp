#include "bitmap.h"

#include <iostream>
#include "exception.h"

bool Bitmap::Load(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        throw ImageException(ImageException::ErrCode::NoFileName);
    }
    Load(file);
    file.close();
    return true;
}

void Bitmap::Load(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&bmp_header_), sizeof(bmp_header_));
    stream.read(reinterpret_cast<char*>(&dib_header_), sizeof(dib_header_));
    if (bmp_header_.signature != 19778) {
        throw ImageException(ImageException::ErrCode::NoBMP);
    }
    const long long RUBBISH_BETWEEN_LINES = (4 - (dib_header_.width * 3) % 4) % 4;
    pixel_array_.SetWidth(dib_header_.width);
    pixel_array_.SetHeight(dib_header_.height);
    for (int32_t i = 0; i < dib_header_.height; ++i) {
        for (int32_t j = 0; j < dib_header_.width; ++j) {
            Pixel pixel{};
            stream.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
            pixel_array_.PushBack(pixel);
        }
        stream.ignore(RUBBISH_BETWEEN_LINES);
    }
}

bool Bitmap::WriteFile(const char* file_name) {
    std::ofstream file;
    file.open(file_name, std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        throw ImageException(ImageException::ErrCode::OutputProblem);
    }
    WriteFile(file);
    file.close();
    return true;
}

bool Bitmap::WriteFile(std::ostream& stream) {
    stream.write(reinterpret_cast<char*> (&bmp_header_), sizeof(bmp_header_));
    stream.write(reinterpret_cast<char*> (&dib_header_), sizeof(dib_header_));
    const long long RUBBISH_BETWEEN_LINES = (4 - (dib_header_.width * 3) % 4) % 4;
    for (int32_t row = 0; row < dib_header_.height; ++row) {
        for (int32_t col = 0; col < dib_header_.width; ++col) {
            stream.write(reinterpret_cast<char*> (&pixel_array_.GetPixel(row, col)),
                    sizeof(pixel_array_.GetPixel(row, col)));
        }
        static const uint32_t rubbish = 0x55AA55AA;
        stream.write(reinterpret_cast<const char*>(&rubbish), RUBBISH_BETWEEN_LINES);
    }
    return true;
}


