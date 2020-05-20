//
// Created by kumquat on 20.05.20.
//

#ifndef MPI_LAB_UTILS_H
#define MPI_LAB_UTILS_H

#include "TwoDimensionalFloatArray.h"

double* read_file_to_array(int& rows, int& cols);

void make_image(TwoDimensionalFloatArray& array, double blue, double red);

#endif //MPI_LAB_UTILS_H
