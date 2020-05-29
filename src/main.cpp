#include <boost/mpi.hpp>
#include "TwoDimensionalDoubleArray.h"
#include <iostream>
#include <string>
#include "utils.h"
#include "Config.h"
#include <Magick++.h>
#include <boost/filesystem.hpp>

namespace mpi = boost::mpi;

int main(int argc, char **argv) {
    mpi::environment env;
    mpi::communicator world;
    boost::mpi::is_mpi_datatype<Config>();
    Config conf{};

    if (world.rank() == 0) {
        if (argc != 3 and argc != 4) {
            std::cout
                    << "You need to specify exactly 2 or 3 arguments: <name of file with configuration> <name of file with matrix> [<name of_output_animation_file>]"
                    << std::endl;
            finish_counter_processes(world);
            return -3;
        }

        std::string config_filename{argv[1]}, matrix_filename{argv[2]};
        std::string animation_filename = argc == 4 ? argv[3] : "animation.gif";

        if (not boost::filesystem::exists(config_filename) || not boost::filesystem::exists(matrix_filename)) {
            std::cout << "One of specified files does not exist. Exiting..." << std::endl;
            finish_counter_processes(world);
            return -5;
        }

        if (get_config_from_file(config_filename, conf) != 0) {
            finish_counter_processes(world);
            return -10;
        }

        // Read the file
        TwoDimensionalDoubleArray matrix;
        read_matrix_from_file(matrix_filename, matrix);
        double blue = matrix.min(), red = matrix.max();

        if (not check_von_neumann(conf.delta_t, conf.delta_x, conf.delta_y, conf.alpha)) {
            std::cout << "Von Neumann inequality is not satisfied! Try to modify the configuration" << std::endl;
            finish_counter_processes(world);
            return -4;
        }

        // Just to prevent some errors in splitting job among processes
        if ((world.size() - 1) * 2 > matrix.rows) {
            finish_counter_processes(world);
            std::cout << "Too many processes for such the amount of rows" << std::endl;
            return 1;
        }


        // Send as equal parts of data as possible to each process
        double step = (double) matrix.rows / (world.size() - 1);
        int cur_row = 0;
        double step_accum = 0;
        int new_row;

        for (int i = 1; i < world.size(); i++) {
            step_accum += step;
            if (i < world.size() - 1)
                new_row = std::min<int>(matrix.rows - 1, (int) step_accum);
            else
                new_row = matrix.rows - 1;

            int rows_data[2] = {new_row - cur_row + 1, matrix.cols};
            world.send(i, 0, rows_data, 2);
            world.send(i, 1, matrix[cur_row], (new_row - cur_row + 1) * matrix.cols);
            world.send(i, 2, conf);
            cur_row = new_row + 1;
        }

        // Main loop. Wait for calculated data and save image
        int metadata[2];
        std::list<Magick::Image> animation;
        for (int iteration = 0; iteration < conf.cycles; iteration += conf.cycles_per_image) {
            cur_row = 1; // Dont need to rewrite the first row

            for (int i = 1; i < world.size(); i++) {
                world.recv(i, 0, metadata, 2); // number of rows, number of columns
                world.recv(i, mpi::any_tag, matrix[cur_row], metadata[0] * metadata[1]);
                cur_row += metadata[0];
            }
            append_image(animation, matrix, blue - 1, red + 1);
        }

        // Create the final animation
        Magick::writeImages(animation.begin(), animation.end(), animation_filename);
        return 0;


    } else {
        // Receive the size of array to process
        int data[2];
        world.recv(0, mpi::any_tag, data, 2);
        if (data[0] == 0 || data[1] == 0)  // (0, 0) is a signal to stop
            return 0;

        int rows = data[0], cols = data[1];
        TwoDimensionalDoubleArray array;

        // Receive the array itself
        // Not beautiful code at all, but this makes everything easier in the latter parts
        if (world.size() == 2) { // Special case when there is only one process for calculations
            array = TwoDimensionalDoubleArray(rows, cols);
            world.recv(0, mpi::any_tag, array[0], rows * cols);
        } else if (world.rank() == 1) {
            array = TwoDimensionalDoubleArray(rows + 1, cols);
            world.recv(0, mpi::any_tag, array[0], rows * cols);
        } else if (world.rank() == world.size() - 1) {
            array = TwoDimensionalDoubleArray(rows + 1, cols);
            world.recv(0, mpi::any_tag, array[1], rows * cols);
        } else {
            array = TwoDimensionalDoubleArray(rows + 2, cols);
            world.recv(0, mpi::any_tag, array[1], rows * cols);
        }

        // Receive the configuration
        world.recv(0, 2, conf);

        // Main loop of calculations
        for (int i = 0; i < conf.cycles; i++) {
            // Exchange the information
            if (world.size() != 2) {
                if (world.rank() == 1) {
                    mpi::request reqs[2];
                    reqs[0] = world.isend(2, 0, array[array.rows - 2], cols);
                    reqs[1] = world.irecv(2, 0, array[array.rows - 1], cols);
                    mpi::wait_all(reqs, reqs + 2);
                } else if (world.rank() == world.size() - 1) {
                    mpi::request reqs[2];
                    reqs[0] = world.isend(world.rank() - 1, 0, array[1], cols);
                    reqs[1] = world.irecv(world.rank() - 1, 0, array[0], cols);
                    mpi::wait_all(reqs, reqs + 2);
                } else {
                    mpi::request reqs[4];
                    // Exchange info with previous
                    reqs[0] = world.isend(world.rank() - 1, 0, array[1], cols);
                    reqs[1] = world.irecv(world.rank() - 1, 0, array[0], cols);

                    // Exchange with the next
                    reqs[2] = world.isend(world.rank() + 1, 0, array[array.rows - 2], cols);
                    reqs[3] = world.irecv(world.rank() + 1, 0, array[array.rows - 1], cols);
                    mpi::wait_all(reqs, reqs + 4);
                }
            }

            update_array(array, conf.delta_x, conf.delta_y, conf.delta_t, conf.alpha);

            // Send array to the 0-st process to create an image
            if (i % conf.cycles_per_image == 0) {
                int metadata[2] = {array.rows - 2, array.cols};
                world.send(0, 0, metadata, 2);
                world.send(0, 0, array[1], metadata[0] * metadata[1]);
            }
        }
    }
    return 0;
}