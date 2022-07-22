cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

add_executable(image_processor
        image_processor.cpp
        cmd_arg_params.cpp
        cmd_arg_params.h
        filter_pipeline_factory.cpp
        filter_pipeline_factory.h
        base_filters.h
        base_filters.cpp
        bitmap.cpp
        bitmap.h
        exception.h
        application.cpp
        application.h)

add_catch(image_processor_test
        test.cpp
        cmd_arg_params.cpp
        application.cpp
        bitmap.cpp
        filter_pipeline_factory.cpp
        base_filters.cpp)
