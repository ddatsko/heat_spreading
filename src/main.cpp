#include <boost/mpi.hpp>
#include "TwoDimensionalDoubleArray.h"
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include "utils.h"

namespace mpi = boost::mpi;


int main() {
    mpi::environment env;
    mpi::communicator world;

    int processes = world.size();

    int cycles_per_output = 10;
    int cycles = 100;
    double delta_t = 0.01;

    if (world.rank() == 0) {
        // Read the file
        std::string filename("matrix");
        TwoDimensionalDoubleArray matrix = read_matrix_from_file(filename);
        int rows = matrix.rows, cols = matrix.cols;


        // Inform unnecessary processes to finish
        if ((processes - 1) * 2 > rows * 2) {
            throw std::invalid_argument("Too many processes for such the amount of rows");
        }


        // Send as equal parts of data as possible to each process
        double step = (double) rows / (processes - 1);
        int cur_row = 0;
        double step_accum = 0;
        int new_row;

        for (int i = 1; i < processes; i++) {
            step_accum += step;
            if (i < processes - 1)
                new_row = std::min<int>(rows - 1, (int) step_accum);
            else
                new_row = rows - 1;

            int rows_data[2] = {new_row - cur_row + 1, cols};
            world.send(i, 0, rows_data, 2);
            world.send(i, 1, matrix[cur_row], (new_row - cur_row + 1) * cols);
            cur_row = new_row + 1;
        }


//        TwoDimensionalDoubleArray result(rows, cols);
//        for (int iteration = 0; iteration < cycles; iteration += cycles_per_output) {
//            cur_row = 0;
//            for (int i = 1; i < processes; ++i) {
//                int metadata[2];
//                world.recv(i, mpi::any_tag, metadata, 2); // number of elements, number of rows
//                world.recv(i, mpi::any_tag, result[cur_row], metadata[0]);
//                cur_row += metadata[1];
//            }
//        }


    } else {
        // Receive the data to process
        int data[2];
        world.recv(0, mpi::any_tag, data, 2);
        if (data[0] == 0 || data[1] == 0)
            return 0;

        int rows = data[0], cols = data[1];


        TwoDimensionalDoubleArray array;

        // Not beautiful code at all, but this makes everything easier in the latter parts
        if (world.size() == 2) { // Special case when there is only one process for calculations
            array = TwoDimensionalDoubleArray(rows, cols);
            world.recv(0, mpi::any_tag, array[0], rows * cols);
        } else if (world.rank() == 1) {
            array = TwoDimensionalDoubleArray(rows + 1, cols);
            world.recv(0, mpi::any_tag, array[1], rows * cols);
        } else if (world.rank() == world.size() - 1) {
            array = TwoDimensionalDoubleArray(rows + 1, cols);
            world.recv(0, mpi::any_tag, array[0], rows * cols);
        } else {
            array = TwoDimensionalDoubleArray(rows + 2, cols);
            world.recv(0, mpi::any_tag, array[1], rows * cols);
        }

        // Main loop of calculations
        for (int i = 0; i < cycles; i++) {
            std::cout << "Cycle " << i << std::endl;
            // Exchange the information
            if (world.size() != 2) {
                if (world.rank() == 1) {
                    mpi::request reqs[2];
                    reqs[0] = world.isend(2, 0, array[array.rows - 2], cols);
                    reqs[1] = world.irecv(2, 0, array[array.rows - 1], cols);
                    mpi::wait_all(reqs, reqs + 2);
                }
                if (world.rank() == world.size() - 1) {
                    mpi::request reqs[2];
                    reqs[0] = world.isend(world.rank() - 1, 0, array[1], cols);
                    reqs[1] = world.irecv(2, 0, array[0], cols);
                    mpi::wait_all(reqs, reqs + 2);
                }
                else {
                    mpi::request reqs[4];
                    // Exchange info with previous
                    reqs[0] = world.isend(world.rank() - 1, 0, array[1], cols);
                    reqs[1] = world.irecv(2, 0, array[0], cols);

                    // Exchange with the next
                    reqs[2] = world.isend(world.rank() + 1, 0, array[array.rows - 2], cols);
                    reqs[3] = world.irecv(world.rank() + 1, 0, array[array.rows - 1], cols);
                }
            }

            array = updated_array(array, 0.1, 0.1, 0.01, 100);

            if (i % cycles_per_output == 0) {
                // TODO: Implement sending info here
            }

        }

    }


    return 0;
}