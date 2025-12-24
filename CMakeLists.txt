cmake_minimum_required(VERSION 4.0)
project(cpp-web-learn)

find_package(Boost 1.78 REQUIRED)

add_executable(main ./main.cpp)
target_link_libraries(main ${Boost_LIBARIES})
