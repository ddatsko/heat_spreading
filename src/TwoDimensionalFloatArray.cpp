#include "TwoDimensionalFloatArray.h"

TwoDimensionalFloatArray::TwoDimensionalFloatArray(int &rows, int &cols): rows{rows}, cols{cols} {
    data = new double[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        data[i] = i;
    }
}

double *TwoDimensionalFloatArray::operator[](int x) {
    return &data[x * cols];
}

TwoDimensionalFloatArray::~TwoDimensionalFloatArray() {
    delete data;
}


