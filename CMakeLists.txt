cmake_minimum_required(VERSION 4.0)

set(BUILD_EXAMPLES ON CACHE BOOL "Build example programs")

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CimpleServer)
find_package(Boost 1.80.0 REQUIRED)
find_package(spdlog 1.8.0 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(cimpleServer ${SOURCES})
target_link_libraries(cimpleServer PUBLIC Boost::boost PRIVATE spdlog)
target_compile_definitions(cimpleServer PRIVATE SPDLOG_USE_STD_FORMAT)
target_include_directories(cimpleServer PUBLIC ./include)

if (${BUILD_EXAMPLES})
    add_subdirectory(example)
endif()
