#ifndef MPI_LAB_UTILS_H
#define MPI_LAB_UTILS_H


#include <boost/mpi/communicator.hpp>
#include "TwoDimensionalDoubleArray.h"
#include "Config.h"
#include <Magick++/Image.h>

void read_matrix_from_file(const std::string &filename, TwoDimensionalDoubleArray& dest);

void update_array(TwoDimensionalDoubleArray &old_array, double delta_x, double delta_y,
                  double delta_t, double alpha);

void append_image(std::list<Magick::Image>& base, TwoDimensionalDoubleArray& array, char* size, double blue, double red);

bool check_von_neumann(double delta_t, double delta_x, double delta_y, double alpha);

int get_config_from_file(std::string &filename, Config &conf);

void finish_counter_processes(boost::mpi::communicator& world);

#endif //MPI_LAB_UTILS_H
