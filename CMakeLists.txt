cmake_minimum_required(VERSION 3.15)
project(bmp_image_processor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Build tests" ON)

if(BUILD_TESTING)
    enable_testing()
    include(CTest)
endif()

add_subdirectory(src)
add_subdirectory(tests)