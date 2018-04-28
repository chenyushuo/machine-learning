#include "matrix.h"

#include <cstdio>
#include <cstring>
#include <ctime>

#include <iostream>

using namespace std;

Matrix :: Matrix(const int &row_number, const int &col_number, const double *value) : 
    row_number_(row_number), col_number_(col_number),
    value_(new double[row_number * col_number])
{
    if (value == nullptr)
        memset(value_, 0, sizeof(double) * row_number * col_number);
    else
        memcpy(value_, value, sizeof(double) * row_number * col_number);
}

Matrix :: Matrix(const Matrix &matrix) :
    row_number_(matrix . row_number_), col_number_(matrix . col_number_),
    value_(new double[matrix . row_number_ * matrix . col_number_])
{
    memcpy(value_, matrix . value_, sizeof(double) * row_number_ * col_number_);
}

Matrix & Matrix :: operator = (const Matrix &matrix){
    if (this == &matrix)
        return *this;
    row_number_ = matrix . row_number_;
    col_number_ = matrix . col_number_;
    delete [] value_;
    new double[row_number_ * col_number_];
    memcpy(value_, matrix . value_, sizeof(double) * row_number_ * col_number_);
    return *this;
}

Matrix :: ~Matrix(){
    delete [] value_;
}

inline Matrix & Matrix :: operator += (const Matrix &matrix){
    Matrix *a = this;
    const Matrix *b = &matrix;
    if (a -> row_number_ != b -> row_number_ || a -> col_number_ != b -> col_number_)
        return *this;
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            a -> Value(i, j) += b -> Value(i, j);    
    return *this;
}

inline Matrix Matrix :: operator + (const Matrix &matrix) const{
    Matrix ans = *this;
    ans += matrix;
    return ans;
}

inline Matrix & Matrix :: operator -= (const Matrix &matrix){
    Matrix *a = this;
    const Matrix *b = &matrix;
    if (a -> row_number_ != b -> row_number_ || a -> col_number_ != b -> col_number_)
        return *this;
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            a -> Value(i, j) -= b -> Value(i, j);    
    return *this;
}

inline Matrix Matrix :: operator - (const Matrix &matrix) const{
    Matrix ans = *this;
    ans -= matrix;
    return ans;
}

Matrix Matrix :: operator * (const Matrix &matrix) const{
    const Matrix *a = this;
    const Matrix *b = &matrix;
    if (a -> col_number_ != b -> row_number_)
        return Matrix();
    Matrix ans = Matrix(a -> row_number_, b -> col_number_);
    for (int i = 0; i < a -> row_number_; i ++)
        for (int k = 0; k < a -> col_number_; k ++){
            double a_i_k = a -> Value(i, k);
            for (int j = 0; j < b -> col_number_; j ++)
                ans . Value(i, j) += a_i_k * b -> Value(k, j);
        }
    return ans;
}

Matrix Matrix :: operator * (const double &k) const{
    Matrix ans = *this;
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            ans . Value(i, j) *= k;
    return ans;
}

Matrix operator * (const double &k, const Matrix &matrix){
    return matrix * k;
}

Matrix & Matrix :: operator /= (const double &k){
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            Value(i, j) /= k;
}

ostream & operator << (ostream &os, const Matrix &matrix){
    os << "row = " << matrix . row_number_ << ", column = " << matrix . col_number_ << endl;
    os << scientific;
    for (int i = 0; i < matrix . row_number_; i ++){
        for (int j = 0; j < matrix . col_number_; j ++)
            os << matrix . Value(i, j) << "  ";
        os << endl;
    }
    return os;
}

inline Matrix Matrix :: Transposition() const{
    Matrix ans = Matrix(col_number_, row_number_);
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            ans . Value(i, j) = Value(j, i);
    return ans;
}

inline void Matrix :: Map(double (*func)(const double &x)){
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            Value(i, j) = (*func)(Value(i, j));
}

inline void Matrix :: random(){
    srand(time(0));
    for (int i = 0; i < row_number_; i ++)
        for (int j = 0; j < col_number_; j ++)
            Value(i, j) = rand() / RAND_MAX;
}

inline RowVector ColVector :: Transposition() const{
    return RowVector(Row(), ValueAddress());
}

inline ColVector RowVector :: Transposition() const{
    return ColVector(Col(), ValueAddress());
}
