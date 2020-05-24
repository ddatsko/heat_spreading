#ifndef MPI_LAB_TWODIMENSIONALDOUBLEARRAY_H
#define MPI_LAB_TWODIMENSIONALDOUBLEARRAY_H




class TwoDimensionalDoubleArray {
private:
    double *data{};

public:
    int rows, cols;

    TwoDimensionalDoubleArray() : rows{0}, cols{0}, data{nullptr} {}

    TwoDimensionalDoubleArray(int rows, int cols);

    TwoDimensionalDoubleArray& operator=(TwoDimensionalDoubleArray &&other) noexcept;

    double *operator[](int x);

    ~TwoDimensionalDoubleArray();
};



#endif //MPI_LAB_TWODIMENSIONALDOUBLEARRAY_H
