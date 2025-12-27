cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-web-learn)
find_package(Boost 1.89 REQUIRED)

add_executable(asyncEchoServer ./example/asyncEchoServer.cpp ./Server.cpp ./Session.cpp ./PacketNode.cpp)
target_include_directories(asyncEchoServer PRIVATE ${CMAKE_SOURCE_DIR})
add_executable(echoServerTestClient ./example/echoServerTestClient.cpp ./PacketNode.cpp)
target_include_directories(echoServerTestClient PRIVATE ${CMAKE_SOURCE_DIR})
