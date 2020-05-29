#include <fstream>
#include <iostream>
#include "utils.h"
#include "TwoDimensionalDoubleArray.h"
#include <Magick++.h>
#include <sstream>
#include <set>


void read_matrix_from_file(const std::string &filename, TwoDimensionalDoubleArray &dest) {
    std::ifstream in(filename);
    std::cin.rdbuf(in.rdbuf());

    int rows, cols;
    std::cin >> rows >> cols;

    TwoDimensionalDoubleArray destination(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            std::cin >> destination[i][j];
    dest = std::move(destination);
}


void update_array(TwoDimensionalDoubleArray &old_array, double delta_x, double delta_y,
                  double delta_t, double alpha) {
    TwoDimensionalDoubleArray new_array(old_array.rows, old_array.cols);
    for (int i = 0; i < new_array.rows; ++i) {
        for (int j = 0; j < new_array.cols; ++j) {
            if (i == 0 || j == 0 || i == new_array.rows - 1 || j == new_array.cols - 1) {
                new_array[i][j] = old_array[i][j];
                continue;
            }
            new_array[i][j] = old_array[i][j] +
                              delta_t * alpha *
                              ((old_array[i - 1][j] - 2 * old_array[i][j] + old_array[i + 1][j]) / (delta_x * delta_x) +
                               (old_array[i][j - 1] - 2 * old_array[i][j] + old_array[i][j + 1]) /
                               (delta_y * delta_y));
        }
    }
    old_array = std::move(new_array);
}


void append_image(std::list<Magick::Image> &base, TwoDimensionalDoubleArray &array, double blue,
                  double red) {
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
    base.emplace_back(img);
}

bool check_von_neumann(double delta_t, double delta_x, double delta_y, double alpha) {
    return  delta_t <  std::max<double>(delta_x, delta_y) * std::max<double>(delta_x, delta_y) / (4 * alpha);
}

void finish_counter_processes(boost::mpi::communicator& world){
    for (int i = 1; i < world.size(); i++) {
        int metadata[2] = {0, 0};
        world.send(i, 0, metadata, 2);
    }
}

int get_config_from_file(std::string &filename, Config &conf) {
    std::ifstream configFile(filename);
    std::string line, key, value;
    double k = 0, ro = 0, c = 0;

    bool alpha_present = false;
    std::set<std::string> required_fields = {"cycles", "cycles_per_image", "delta_t", "delta_x", "delta_y"};
    while (getline(configFile, line)) {
        if (line.empty() || line[0] == '#' || line[0] == ' ')
            continue;
        std::istringstream lineStream(line);

        if (std::getline(lineStream, key, '=')) {
            if (std::getline(lineStream, value)) {
                if (key == "cycles")
                    conf.cycles = std::stoi(value);
                if (key == "cycles_per_image")
                    conf.cycles_per_image = std::stoi(value);
                if (key == "delta_t")
                    conf.delta_t = std::stof(value);
                if (key == "delta_x")
                    conf.delta_x = std::stof(value);
                if (key == "delta_y")
                    conf.delta_y = std::stof(value);
                if (key == "ro")
                    ro = std::stof(value);
                if (key == "k")
                    k = std::stof(value);
                if (key == "c")
                    c = std::stof(value);
                if (key == "alpha") {
                    alpha_present = true;
                    conf.alpha = std::stof(value);
                }
                if (required_fields.find(key) != required_fields.end()) {
                    required_fields.erase(key);
                }
            }
        }
    }
    if (!required_fields.empty()) {
        std::cout << "These fields are required, but not present in the config file" << std::endl;
        for (auto &s: required_fields) {
            std::cout << s << ", ";
        }
        std::cout << std::endl;
        return -1;
    }
    if (not alpha_present && (ro * k * c) == 0) {
        std::cout << "You should specify either 'alpha' or not 0 'ro', 'k' and 'c' fields in config file" << std::endl;
        return -2;
    } else if (not alpha_present) {
        conf.alpha = k / (ro * c);
    }
    if (conf.cycles <= 0 || conf.cycles_per_image <= 0) {
        std::cout << "'cycles' and 'cycles_per_image' should be positive numbers" << std::endl;
        return -3;
    }
    return 0;

}

