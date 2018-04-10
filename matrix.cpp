#include "matrix.h"

#include <cstdio>
#include <cstring>

using namespace std;

Matrix :: Matrix(const int &col_number, const int &row_number, double *value) : 
    col_number_(col_number), row_number_(row_number),
    value_(new double[col_number * row_number])
{
    if (value == nullptr)
        memset(value_, 0, sizeof(double) * col_number * row_number);
    else
        memcpy(value_, value, sizeof(double) * col_number * row_number);
}

Matrix :: Matrix(const Matrix &matrix) :
    col_number_(matrix . col_number_), row_number_(matrix . row_number_),
    value_(new double[matrix . col_number_ * matrix . row_number_])
{
    memcpy(value_, matrix . value_, sizeof(double) * col_number_ * row_number_);
}

Matrix & Matrix :: operator = (const Matrix &matrix){
    if (this == &matrix)
        return *this;
    col_number_ = matrix . col_number_;
    row_number_ = matrix . row_number_;
    delete [] value_;
    new double[col_number_ * row_number_];
    memcpy(value_, matrix . value_, sizeof(double) * col_number_ * row_number_);
    return *this;
}

Matrix :: ~Matrix(){
    delete [] value_;
}


