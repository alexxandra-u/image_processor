#pragma once

#include "base_filters.h"
#include "cmd_arg_params.h"
#include "map"

namespace FilterFactories {
BaseFilter* MakeBlurFilter(const FilterDescriptor& fd);

BaseFilter* MakeCropFilter(const FilterDescriptor& fd);

BaseFilter* MakeNegativeFilter(const FilterDescriptor& fd);

BaseFilter* MakeGreyscaleFilter(const FilterDescriptor& fd);

BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd);

BaseFilter* MakeEdgeDetectionFilter(const FilterDescriptor& fd);

BaseFilter* MakeBlocksFilter(const FilterDescriptor& fd);
}

class FilterPipeline {
public:
    bool Apply(Bitmap& bitmap);
    bool Clear();
    ~FilterPipeline() = default;
public:
    std::vector<BaseFilter*> vector_of_filters;
};

class FilterPipelineFactory {
public:
    using FilterFactory = BaseFilter* (*)(const FilterDescriptor&);
    using FilterToMakerMap = std::map<std::string_view, FilterFactory>;
public:
    FilterPipelineFactory() = default;
    FilterPipelineFactory(FilterToMakerMap map) : filter_to_makers_(map) {};

    void AddFilterMaker(std::string_view name, FilterFactory factory);

    FilterFactory GetFilterFactoryMaker(std::string_view name) const;

    BaseFilter* CreateFilter(const FilterDescriptor& descriptor) const;

    FilterPipeline* MakeFilterPipeline(std::vector<FilterDescriptor> fdvector);

protected:
    FilterToMakerMap filter_to_makers_;
};







