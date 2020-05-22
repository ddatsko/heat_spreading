//
// Created by kumquat on 20.05.20.
//

#ifndef MPI_LAB_UTILS_H
#define MPI_LAB_UTILS_H

#include "TwoDimensionalDoubleArray.h"

double *read_file_to_array(int &rows, int &cols);

void make_image(TwoDimensionalDoubleArray &array, double blue, double red);

TwoDimensionalDoubleArray read_matrix_from_file(const std::string &filename);

TwoDimensionalDoubleArray updated_array(TwoDimensionalDoubleArray &old_array, double delta_x, double delta_y,
                                        double delta_t, double alpha);

#endif //MPI_LAB_UTILS_H
