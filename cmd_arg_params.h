#pragma once

#include <string_view>
#include <vector>

struct FilterDescriptor {
    std::string_view filter_name;
    std::vector<std::string_view> filter_params;
};

class CmdLineParser {
public:
    static const int MIN_PARAM_NUM = 3;
    static const int INPUT_FILE_NAME_POS_ = 1;
    static const int OUTPUT_FILE_NAME_POS_ = 2;
public:
    bool Parse(int argc, char* argv[]);
    std::string_view GetInputFileName() const { return input_file_name_; }
    std::string_view GetOutputFileName() const {return output_file_name_;}
    std::vector<FilterDescriptor> GetFilterDescriptorsVector() const {return
    filter_descriptors_vector; }

protected:
    std::string_view input_file_name_;
    std::string_view output_file_name_;
    std::vector<FilterDescriptor> filter_descriptors_vector;
    const std::string_view start_message = "Please enter input file name, output "
                                               "file "
                                         "name, filter names and filter "
                                     "parameters to use this app. Filters available: \n"
                                     " -neg (takes no arguments) \n"
                                     " -gs (takes no arguments) \n"
                                     " -blur (takes 1 argument) \n"
                                     " -sharp (takes no arguments) \n"
                                     " -edge (takes no arguments) \n"
                                     " -blocks (takes 2 arguments)";
};
