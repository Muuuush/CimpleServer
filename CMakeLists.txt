cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cimpleServer)
find_package(Boost 1.89 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(cimpleServer ${SOURCES})
target_include_directories(cimpleServer PUBLIC ./include)

add_executable(hello-server ./example/hello/server.cpp)
target_link_libraries(hello-server PUBLIC cimpleServer)
add_executable(hello-client ./example/hello/client.cpp)
target_link_libraries(hello-client PUBLIC cimpleServer)
