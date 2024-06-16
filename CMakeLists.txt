cmake_minimum_required(VERSION 3.28)
project(CompileArray)

set(CMAKE_CXX_STANDARD 20)
include(${ENV})

find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(CompileArray main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")