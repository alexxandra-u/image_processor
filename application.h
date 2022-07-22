#pragma once

#include "bitmap.h"
#include "cmd_arg_params.h"
#include "exception.h"
#include "filter_pipeline_factory.h"

class Application{
public:
    bool Run(int argc, char* argv[]);

protected:
    std::map<std::string_view, BaseFilter* (*)(const FilterDescriptor&)>
    filter_to_makers_map = {{"crop", &FilterFactories::MakeCropFilter},
            {"neg", &FilterFactories::MakeNegativeFilter},
            {"gs", &FilterFactories::MakeGreyscaleFilter},
            {"sharp", &FilterFactories::MakeSharpeningFilter},
            {"edge", &FilterFactories::MakeEdgeDetectionFilter},
            {"blur", &FilterFactories::MakeBlurFilter},
            {"blocks", &FilterFactories::MakeBlocksFilter}};

    std::map<ImageException::ErrCode, std::string_view> exceptions_to_messages_ = {
            {ImageException::ErrCode::NoBMP, "This is not a BMP file"},
            {ImageException::ErrCode::NoFileName, "Input file was not found"},
            {ImageException::ErrCode::NoSuchFilter, "There is no such a filter"},
            {ImageException::ErrCode::WrongEdgeParams, "Wrong edge detection filter params"},
            {ImageException::ErrCode::WrongSharpParams, "Wrong sharpening filter params"},
            {ImageException::ErrCode::WrongGsParams, "Wrong grayscale filter params"},
            {ImageException::ErrCode::WrongNegParams, "Wrong negative filter params"},
            {ImageException::ErrCode::WrongCropParams, "Wrong crop filter params"},
            {ImageException::ErrCode::WrongBlurParams, "Wrong blur filter params"},
            {ImageException::ErrCode::WrongBlocksParams, "Wrong blocks filter params"},
            {ImageException::ErrCode::OutputProblem, "Output file opening problem"}};
};
