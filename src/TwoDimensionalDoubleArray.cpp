#include <iostream>
#include "TwoDimensionalDoubleArray.h"

TwoDimensionalDoubleArray::TwoDimensionalDoubleArray(int rows, int cols): rows{rows}, cols{cols} {
    data = new double[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        data[i] = 0;
    }
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


