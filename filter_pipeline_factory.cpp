#include "exception.h"
#include "filter_pipeline_factory.h"
#include <stdexcept>


namespace FilterFactories {
    BaseFilter* MakeBlurFilter(const FilterDescriptor& fd) {
        if (fd.filter_params.size() != 1) {
            throw ImageException(ImageException::ErrCode::WrongBlurParams);
        }
        BlurFilter* blur_filter = new BlurFilter;
        std::string string = static_cast<std::string>(fd.filter_params[0]);
        blur_filter->sigma = std::stof(string);
        return blur_filter;
    }

    BaseFilter* MakeCropFilter(const FilterDescriptor& fd) {
        if (fd.filter_params.size() != 2) {
            throw ImageException(ImageException::ErrCode::WrongCropParams);
        }
        CropFilter* crop_filter = new CropFilter;
        std::string str_width = static_cast<std::string>(fd.filter_params[0]);
        std::string str_height = static_cast<std::string>(fd.filter_params[1]);
        crop_filter->width_ = std::stoi(str_width);
        crop_filter->height_ = std::stoi(str_height);
        return crop_filter;
    }

    BaseFilter* MakeNegativeFilter(const FilterDescriptor& fd) {
        if (!fd.filter_params.empty()) {
            throw ImageException(ImageException::ErrCode::WrongNegParams);
        }
        return new NegativeFilter;
    }

    BaseFilter* MakeGreyscaleFilter(const FilterDescriptor& fd) {
        if (!fd.filter_params.empty()) {
            throw ImageException(ImageException::ErrCode::WrongGsParams);
        }
        return new GreyscaleFilter;
    }

    BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd) {
        if (!fd.filter_params.empty()) {
            throw ImageException(ImageException::ErrCode::WrongSharpParams);
        }
        return new SharpeningFilter;
    }

    BaseFilter* MakeEdgeDetectionFilter(const FilterDescriptor& fd) {
        if (fd.filter_params.size() != 1) {
            throw ImageException(ImageException::ErrCode::WrongEdgeParams);
        }
        EdgeDetectionFilter* edge_filter = new EdgeDetectionFilter;
        std::string str = static_cast<std::string>(fd.filter_params[0]);
        edge_filter->threshold_ = std::stoi(str);
        return edge_filter;
    }

    BaseFilter* MakeBlocksFilter(const FilterDescriptor& fd) {
        if (fd.filter_params.size() != 2) {
            throw ImageException(ImageException::ErrCode::WrongBlocksParams);
        }
        BlocksFilter* blocks_filter = new BlocksFilter;
        std::string x = static_cast<std::string>(fd.filter_params[0]);
        std::string y = static_cast<std::string>(fd.filter_params[1]);
        blocks_filter->x_ = std::stoi(x);
        blocks_filter->y_ = std::stoi(y);
        return blocks_filter;
    }
}

void FilterPipelineFactory::AddFilterMaker(std::string_view name, FilterFactory factory) {
    filter_to_makers_.insert({name, factory});
}

FilterPipelineFactory::FilterFactory FilterPipelineFactory::GetFilterFactoryMaker(std::string_view name) const {
    FilterToMakerMap::const_iterator it = filter_to_makers_.find(name);
    if (it != filter_to_makers_.end()) {
        return it->second;
    }
    return nullptr;
}

BaseFilter* FilterPipelineFactory::CreateFilter(const FilterDescriptor& descriptor) const {
    FilterFactory gear = GetFilterFactoryMaker(descriptor.filter_name);
    if (!gear) {
        throw ImageException(ImageException::ErrCode::NoSuchFilter);
    }
    BaseFilter* current_filter = gear(descriptor);
    return current_filter;
}

FilterPipeline* FilterPipelineFactory::MakeFilterPipeline(std::vector<FilterDescriptor>
        fdvector) {
    FilterPipeline* fp = new FilterPipeline;
    for (FilterDescriptor descriptor : fdvector) {
        BaseFilter* filter = CreateFilter(descriptor);
        fp->vector_of_filters.emplace_back(filter);
    }
    return fp;
}


bool FilterPipeline::Apply(Bitmap& bitmap) {
    for (BaseFilter* filter : vector_of_filters) {
        try {
            filter->Apply(bitmap);
        } catch (...) {
            return false;
        }
    }
    return true;
}

bool FilterPipeline::Clear() {
    for (BaseFilter* filter : vector_of_filters) {
        try {
            delete filter;
        } catch (...) {
            return false;
        }
    }
    return true;
}