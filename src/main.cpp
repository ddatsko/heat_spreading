#include <boost/mpi.hpp>
#include "TwoDimensionalFloatArray.h"
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include "utils.h"

namespace mpi = boost::mpi;

int main()
{

    int rows = 2, cols = 100;
    TwoDimensionalFloatArray array(rows, cols);
    make_image(array, -10, 110);

    return 0;
}