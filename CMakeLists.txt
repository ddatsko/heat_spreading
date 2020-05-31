cmake_minimum_required(VERSION 3.15)
project(mpi_lab)

set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(mpi_lab src/main.cpp src/utils.cpp src/utils.h src/TwoDimensionalDoubleArray.cpp src/TwoDimensionalDoubleArray.h src/Config.cpp src/Config.h)

FIND_PACKAGE(Boost COMPONENTS mpi filesystem REQUIRED)
TARGET_LINK_LIBRARIES(mpi_lab Boost::mpi Boost::filesystem)


# Link Magick++
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
find_package(ImageMagick REQUIRED COMPONENTS Magick++ MagickWand MagickCore)

include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(mpi_lab ${ImageMagick_LIBRARIES})
