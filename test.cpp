#include "contrib/catch/catch.hpp"

#include "application.h"
#include <iostream>

TEST_CASE("Filter descriptor creation") {
    FilterDescriptor desc1{"blur", {"12", "23", "156"}};
    REQUIRE(desc1.filter_name == "blur");
    REQUIRE(desc1.filter_params[0] == "12");
}

TEST_CASE("Filter pipeline factory creation") {
    FilterPipelineFactory fpf;
    fpf.AddFilterMaker("blur", &FilterFactories::MakeBlurFilter);
    fpf.AddFilterMaker("crop", &FilterFactories::MakeCropFilter);
    FilterPipelineFactory::FilterFactory gear = fpf.GetFilterFactoryMaker("blur");
    REQUIRE(gear);
    FilterPipelineFactory::FilterFactory non_existing_gear = fpf.GetFilterFactoryMaker("alpha");
    REQUIRE(!non_existing_gear);
}

TEST_CASE("File loading") {
    const char* input_filename = "../examples/example.bmp";
    Bitmap bitmap{};
    REQUIRE(bitmap.Load(input_filename));
}

TEST_CASE("File loading and file writing") {
    const char* input_filename = "../examples/example.bmp";
    Bitmap bitmap{};
    if (!bitmap.Load(input_filename)) {
        std::cout << "bitmap loading mistake" << std::endl;
    }
    const char* output_filename = "../output_example.bmp";
    if (!bitmap.WriteFile(output_filename)) {
        std::cout << "bitmap writing mistake" << std::endl;
    }
}

TEST_CASE("Negative filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"neg", {}};
    fpf.AddFilterMaker("neg", &FilterFactories::MakeNegativeFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}

TEST_CASE("Greyscale filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"gs", {}};
    fpf.AddFilterMaker("gs", &FilterFactories::MakeGreyscaleFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}

TEST_CASE("Crop filter with good params") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"crop", {"1000", "800"}};
    fpf.AddFilterMaker("crop", &FilterFactories::MakeCropFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}

TEST_CASE("Crop filter with bad params") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"crop", {"4000", "3000"}};
    fpf.AddFilterMaker("crop", &FilterFactories::MakeCropFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
    delete filter;
}

TEST_CASE("Sharpening filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"sharp", {}};
    fpf.AddFilterMaker("sharp", &FilterFactories::MakeSharpeningFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));

}

TEST_CASE("Edge filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"edge", {"5"}};
    fpf.AddFilterMaker("edge", &FilterFactories::MakeEdgeDetectionFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}

TEST_CASE("Blur filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"blur", {"2"}};
    fpf.AddFilterMaker("blur", &FilterFactories::MakeBlurFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));

}

TEST_CASE("Blocks filter") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"blocks", {"18", "32"}};
    fpf.AddFilterMaker("blocks", &FilterFactories::MakeBlocksFilter);
    BaseFilter* filter = fpf.CreateFilter(desc1);
    REQUIRE(filter);

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(filter->Apply(bitmap));
    delete filter;

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}

TEST_CASE("Filter pipeline creation") {
    FilterPipelineFactory fpf;
    FilterDescriptor desc1{"blocks", {"18", "32"}};
    FilterDescriptor desc2{"crop", {"1000", "2000"}};
    fpf.AddFilterMaker("blocks", &FilterFactories::MakeBlocksFilter);
    fpf.AddFilterMaker("crop", &FilterFactories::MakeCropFilter);
    FilterPipeline* fp = fpf.MakeFilterPipeline({desc1, desc2});

    Bitmap bitmap;
    REQUIRE(bitmap.Load("../examples/example.bmp"));
    REQUIRE(fp->Apply(bitmap));

    REQUIRE(bitmap.WriteFile("../output_example.bmp"));
}