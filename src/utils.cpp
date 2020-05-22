#include <fstream>
#include <iostream>
#include "utils.h"
#include "TwoDimensionalDoubleArray.h"
#include <Magick++.h>


TwoDimensionalDoubleArray read_matrix_from_file(const std::string &filename) {
    std::ifstream in(filename);
    std::cin.rdbuf(in.rdbuf());

    int rows, cols;
    std::cin >> rows >> cols;
    TwoDimensionalDoubleArray destination(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            std::cin >> destination[i][j];
    return destination;
}


TwoDimensionalDoubleArray updated_array(TwoDimensionalDoubleArray &old_array, double delta_x, double delta_y,
                                        double delta_t, double alpha) {
    TwoDimensionalDoubleArray new_array(old_array.rows, old_array.cols);
    for (int i = 0; i < new_array.rows; ++i) {
        for (int j = 0; j < new_array.cols; ++j) {
            if (i == 0 || j == 0 || i == new_array.rows - 1 || j == new_array.cols - 1) {
                new_array[i][j] = old_array[i][j];
                continue;
            }
            new_array[i][j] = old_array[i][j] +
            delta_t * alpha * (((old_array[i - 1][j] - 2 * old_array[i][j]) / (delta_x * delta_x) +
                                old_array[i + 1][j]) +
                               ((old_array[i][j - 1] - 2 * old_array[i][j] + old_array[i][j + 1]) /
                                (delta_y * delta_y)));
        }
    }
    return new_array;
}


void make_image(TwoDimensionalDoubleArray &array, double blue, double red) {
    char size[100];
    sprintf(size, "%dx%d", array.cols, array.rows);
    Magick::Image img(size, "white");
    for (int i = 0; i < array.rows; i++) {
        for (int j = 0; j < array.cols; j++) {
            double value = array[i][j], color;
            if (value <= blue) {
                color = 0.66;
            } else if (value >= red) {
                color = 0;
            } else {
                color = 0.66 - 0.66 * (value - blue) / (red - blue);
            }

            img.pixelColor(j, i, Magick::ColorHSL(color, 1, 0.5));
        }
    }
    img.write("out.gif");
}


