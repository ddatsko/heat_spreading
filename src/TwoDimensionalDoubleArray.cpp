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


