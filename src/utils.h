//
// Created by kumquat on 20.05.20.
//

#ifndef MPI_LAB_UTILS_H
#define MPI_LAB_UTILS_H

#include <Magick++/Image.h>
#include "TwoDimensionalDoubleArray.h"

double *read_file_to_array(int &rows, int &cols);

void make_image(TwoDimensionalDoubleArray &array, std::string filename, double blue, double red);

void read_matrix_from_file(const std::string &filename, TwoDimensionalDoubleArray& dest);

void update_array(TwoDimensionalDoubleArray &old_array, double delta_x, double delta_y,
                  double delta_t, double alpha);

void append_image(std::list<Magick::Image>& base, TwoDimensionalDoubleArray& array, std::string filename, double blue, double red);


#endif //MPI_LAB_UTILS_H
