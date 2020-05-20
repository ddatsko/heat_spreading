#ifndef MPI_LAB_TWODIMENSIONALFLOATARRAY_H
#define MPI_LAB_TWODIMENSIONALFLOATARRAY_H


class TwoDimensionalFloatArray {
private:
    double *data;

public:
    int rows, cols;
    TwoDimensionalFloatArray(int &rows, int &cols);

    double* operator[](int x);

    ~TwoDimensionalFloatArray();
};


#endif //MPI_LAB_TWODIMENSIONALFLOATARRAY_H
