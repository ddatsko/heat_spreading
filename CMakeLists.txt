cmake_minimum_required(VERSION 3.15)
project(mpi_lab)

set(CMAKE_CXX_STANDARD 20)

add_executable(mpi_lab src/main.cpp src/utils.cpp src/utils.h src/TwoDimensionalDoubleArray.cpp src/TwoDimensionalDoubleArray.h)

FIND_PACKAGE(MPI)
message(${MPI_INCLUDE_PATH})


# TODO: remove version from this line or place a lower one
FIND_PACKAGE(Boost 1.73 COMPONENTS mpi REQUIRED)
TARGET_LINK_LIBRARIES(mpi_lab Boost::mpi)



# Link Magick++
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick REQUIRED COMPONENTS Magick++ MagickWand MagickCore)

include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(mpi_lab ${ImageMagick_LIBRARIES})
