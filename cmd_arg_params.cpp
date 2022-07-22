#include "cmd_arg_params.h"
#include "exception.h"
#include <iostream>

bool CmdLineParser::Parse(int argc, char* argv[]) {
    if (argc < MIN_PARAM_NUM) {
        std::cout << start_message << std::endl;
        throw ImageException(ImageException::ErrCode::NoFileName);
    }
    if (argc == MIN_PARAM_NUM) {
        std::cout << start_message << std::endl;
        throw ImageException(ImageException::ErrCode::NoFilters);
    }
    input_file_name_ = argv[INPUT_FILE_NAME_POS_];
    output_file_name_ = argv[OUTPUT_FILE_NAME_POS_];
    FilterDescriptor a;
    for (int i = OUTPUT_FILE_NAME_POS_ + 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (i != OUTPUT_FILE_NAME_POS_ + 1) {
                filter_descriptors_vector.push_back(a);
            }
            a.filter_name = std::string_view(argv[i]).substr(1, std::string_view(argv[i]).size());
            a.filter_params.clear();
        } else {
            a.filter_params.push_back(std::string_view(argv[i]));
        }
    }
    filter_descriptors_vector.emplace_back(a);
    return true;
}

