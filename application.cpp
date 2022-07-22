#include "application.h"
#include <iostream>

bool Application::Run(int argc, char* argv[]) {
    CmdLineParser parser;
    try {
        parser.Parse(argc, argv);
    } catch (ImageException& e) {
        std::cout << exceptions_to_messages_[e.GetExceptionType()] << std::endl;
        return false;
    } catch (...) {
        std::cout << "parsing problem" << std::endl;
        return false;
    }

    Bitmap bitmap;
    std::string input_filename = static_cast<std::string>(parser.GetInputFileName());
    try {
        bitmap.Load(input_filename.c_str());
    } catch (ImageException& e) {
        std::cout << exceptions_to_messages_[e.GetExceptionType()] << std::endl;
        return false;
    } catch (...) {
        std::cout << "Bitmap loading problem" << std::endl;
        return false;
    }

    FilterPipelineFactory fpf(filter_to_makers_map);
    try {
        FilterPipeline* fp = fpf.MakeFilterPipeline(parser.GetFilterDescriptorsVector());
        fp->Apply(bitmap);
        fp->Clear();
        delete fp;
    } catch (ImageException& e) {
        std::cout << exceptions_to_messages_[e.GetExceptionType()] << std::endl;
        return false;
    } catch (...) {
        std::cout << "filter application problem" << std::endl;
        return false;
    }

    std::string output_filename = static_cast<std::string>(parser.GetOutputFileName());
    try {
        bitmap.WriteFile(output_filename.c_str());
    } catch (ImageException& e) {
        std::cout << exceptions_to_messages_[e.GetExceptionType()] << std::endl;
        return false;
    } catch (...) {
        std::cout << "file writing problem" << std::endl;
        return false;
    }
    return true;
}
