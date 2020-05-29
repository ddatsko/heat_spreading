#include <iostream>
#include "TwoDimensionalDoubleArray.h"

TwoDimensionalDoubleArray::TwoDimensionalDoubleArray(int rows, int cols): rows{rows}, cols{cols} {
    data = new double[rows * cols];
}

double *TwoDimensionalDoubleArray::operator[](int x) {
    return &data[x * cols];
}

TwoDimensionalDoubleArray::~TwoDimensionalDoubleArray() {
    delete data;
}

TwoDimensionalDoubleArray &TwoDimensionalDoubleArray::operator=(TwoDimensionalDoubleArray &&other) noexcept {
    delete this->data;
    this->data = other.data;
    other.data = nullptr;
    this->rows = other.rows;
    this->cols = other.cols;
    return *this;
}

double TwoDimensionalDoubleArray::min() {
    double min_val = std::numeric_limits<double>::infinity();
    for (int i = 0; i < rows * cols; i++) {
        min_val = std::min<double>(data[i], min_val);
    }
    return min_val;
}

double TwoDimensionalDoubleArray::max() {
    double max_val = -std::numeric_limits<double>::infinity();
    for (int i = 0; i < rows * cols; i++) {
        max_val = std::max<double>(data[i], max_val);
    }
    return max_val;
}


