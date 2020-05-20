#include <fstream>
#include <iostream>
#include "utils.h"
#include "TwoDimensionalFloatArray.h"
#include <Magick++.h>

double *read_file_to_array(int &rows, int &cols) {
    std::ifstream in("in.txt");
    std::streambuf *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    std::cin >> rows >> cols;
    auto res = new double[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        std::cin >> res[i];
    }
    return res;

}

void make_image(TwoDimensionalFloatArray& array, double blue, double red) {
    char size[100];
    sprintf(size, "%dx%d", array.cols, array.rows);
    Magick::Image img( size, "white" );
    for (int i = 0; i < array.rows; i++) {
        for (int j = 0; j < array.cols; j++) {
            double value = array[i][j], pixel_red;
            std::cout << value << std::endl;
            if (value <= blue) {
                pixel_red = 0;
            } else if (value >= red) {
                pixel_red = 1.0;
            } else {
                pixel_red = (value - blue) / (red - blue);
            }
            std::cout << pixel_red << std::endl;

            img.pixelColor(j, i, Magick::ColorRGB(pixel_red, 0.32, 1.0 - pixel_red));
        }
    }
    img.write("out.gif");
}
