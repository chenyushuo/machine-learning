#include "matrix.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstddef>

#include <iostream>
#include <algorithm>

using namespace std;

Matrix :: Matrix(const int &row_number, const int &col_number) :
    row_number_(row_number), col_number_(col_number),
    value_(new double [row_number * col_number]())
{

}

Matrix :: Matrix(const int &row_number, const int &col_number, const double *value) :
    row_number_(row_number), col_number_(col_number),
    value_(new double [row_number * col_number])
{
    memcpy(value_, value, sizeof(double) * row_number * col_number);
}

Matrix :: Matrix(const Matrix &matrix) :
    row_number_(matrix . row_number_), col_number_(matrix . col_number_),
    value_(new double [matrix . row_number_ * matrix . col_number_])
{
    memcpy(value_, matrix . value_, sizeof(double) * row_number_ * col_number_);
}

Matrix & Matrix :: operator = (const Matrix &matrix){
    if (this == &matrix)
        return *this;
    row_number_ = matrix . row_number_;
    col_number_ = matrix . col_number_;
    delete [] value_;
    value_ = new double [row_number_ * col_number_];
    memcpy(value_, matrix . value_, sizeof(double) * row_number_ * col_number_);
    return *this;
}

Matrix :: ~Matrix(){
    delete [] value_;
}

Matrix & Matrix :: operator += (const Matrix &matrix){
    const Matrix *b = &matrix;
#ifdef DEBUG_MODE
    Matrix *a = this;
    if (a -> row_number_ != b -> row_number_ || a -> col_number_ != b -> col_number_)
        exit(1);
#endif
    double *va = value_;
    double *vb = b -> value_;
    for (int i = 0; i < row_number_ * col_number_; i ++){
        *va += *vb;
        va ++, vb ++;
    }
    return *this;
}

Matrix Matrix :: operator + (const Matrix &matrix) const{
    Matrix ans = *this;
    ans += matrix;
    return ans;
}

Matrix & Matrix :: operator -= (const Matrix &matrix){
    const Matrix *b = &matrix;
#ifdef DEBUG_MODE
    Matrix *a = this;
    if (a -> row_number_ != b -> row_number_ || a -> col_number_ != b -> col_number_)
        exit(1);
#endif
    double *va = value_;
    double *vb = b -> value_;
    for (int i = 0; i < row_number_ * col_number_; i ++){
        *va -= *vb;
        va ++, vb ++;
    }
    return *this;
}

Matrix Matrix :: operator - (const Matrix &matrix) const{
    Matrix ans = *this;
    ans -= matrix;
    return ans;
}

Matrix Matrix :: operator * (const Matrix &matrix) const{
    const Matrix *a = this;
    const Matrix *b = &matrix;
#ifdef DEBUG_MODE
    if (a -> col_number_ != b -> row_number_)
        exit(1);
#endif
    Matrix ans = Matrix(a -> row_number_, b -> col_number_);
    double *va = value_;
    for (int i = 0; i < a -> row_number_; i ++){
        const double *vb = b -> value_;
        for (int k = 0; k < a -> col_number_; k ++){
            double a_i_k = *va ++;
            double *vans = ans.value_ + i * ans.col_number_;
            for (int j = 0; j < b -> col_number_; j ++){
                *vans += a_i_k * (*vb);
                vans ++, vb ++;
            }
        }
    }
    return ans;
}

Matrix & Matrix :: operator *= (const double &k){
    transform(value_, value_ + row_number_ * col_number_, value_,
              [k] (const double &x){return x * k;});
    return *this;
}

void Matrix :: InputMulti(const Matrix &theta, const Matrix &output){
    double *vi = value_;
    int len = output.row_number_;
    const double *vt = theta.value_;
    const double *vo = output.value_;
    for (int i = 0; i < row_number_; i ++){
        double tmp = 0;
        for (int j = 0; j < len; j ++){
            tmp += (*vo) * (*vt);
            vo ++, vt ++;
        }
        vo -= len;
        *vi = tmp, vi ++;
    }
}

void Matrix :: DeltaMulti(const Matrix &delta, const Matrix &theta){
    clear();
    const double *vd2 = delta.value_;
    const double *vt = theta.value_;
    for (int i = 0; i < delta.col_number_; i ++){
        double *vd = value_;
        double k = *vd2;
        for (int j = 0; j < col_number_; j ++){
            *vd += k * (*vt);
            vd ++, vt ++;
        }
        vd2 ++;
    }
}

void Matrix :: GradThetaMulti(const Matrix &delta_T, const Matrix &output_T){
    double *vgt = value_;
    const double *vd = delta_T.value_;
    const double *vo = output_T.value_;
    for (int i = 0; i < row_number_; i ++){
        double k = *vd;
        for (int j = 0; j < col_number_; j ++){
            *vgt += k * (*vo);
            vgt ++, vo ++;
        }
        vo -= col_number_;
        vd ++;
    }
}

Matrix & Matrix :: operator /= (const double &k){
    transform(value_, value_ + row_number_ * col_number_, value_,
              [k] (const double &x){return x / k;});
    return *this;
}

ostream & operator << (ostream &os, const Matrix &matrix){
    os << "row = " << matrix . row_number_ <<
          ", column = " << matrix . col_number_ << endl;
    os << scientific;
    for (int i = 0; i < matrix . row_number_; i ++){
        for (int j = 0; j < matrix . col_number_; j ++)
            os << matrix . Value(i, j) << "  ";
        os << endl;
    }
    return os;
}

Matrix Matrix :: Transposition() const{
    Matrix ans = Matrix(col_number_, row_number_);
    double *vans = ans.value_;
    double *v = value_;
    for (int i = 0; i < col_number_; i ++){
        v = value_ + i;
        for (int j = 0; j < row_number_; j ++){
            *vans = *v;
            vans ++, v += col_number_;
        }
    }
    return ans;
}

void Matrix :: Map(double (*func)(const double &x)){
    transform(value_, value_ + row_number_ * col_number_, value_, func);
}

void Matrix :: random(){
    srand(rand() + time(0));
    transform(value_, value_ + row_number_ * col_number_, value_,
              [] (const double &x){return 0.1 * (rand() - rand()) / RAND_MAX;});
}

void Matrix :: Save(FILE *fp){
    fwrite(value_, sizeof(double) * row_number_ * col_number_, 1, fp);
}

void Matrix :: Load(FILE *fp){
    fread(value_, sizeof(double) * row_number_ * col_number_, 1, fp);
}

RowVector ColVector :: Transposition() const{
    return RowVector(Row(), ValueAddress());
}

ColVector RowVector :: Transposition() const{
    return ColVector(Col(), ValueAddress());
}
