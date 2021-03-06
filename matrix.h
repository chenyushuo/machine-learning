#ifndef MATRIX_H
#define MATRIX_H

#include <cstdio>
#include <cstring>
#include <cstddef>

#include <iostream>

using std :: ostream;

class Matrix{
private :
    int row_number_, col_number_;
    double *value_;

public :
    Matrix() : row_number_(0), col_number_(0), value_(nullptr){}
    Matrix(const int &row_number, const int &col_number);
    Matrix(const int &row_number, const int &col_number,
           const double *value);
    Matrix(const Matrix &matrix);

    virtual ~Matrix();

    const double *ValueAddress() const{return value_;}

    const int &Row() const{return row_number_;}
    const int &Col() const{return col_number_;}

    virtual double &Value(int i, int j){
        return value_[i * col_number_ + j];
    }
    virtual const double &Value(int i, int j) const{
        return value_[i * col_number_ + j];
    }

    Matrix & operator = (const Matrix &matrix);

    Matrix & operator += (const Matrix &matrix);
    Matrix operator + (const Matrix &matrix) const;

    Matrix & operator -= (const Matrix &matrix);
    Matrix operator - (const Matrix &matrix) const;

    Matrix operator * (const Matrix &matrix) const;
    Matrix & operator *= (const double &k);
    void InputMulti(const Matrix &theta, const Matrix &output);
    void DeltaMulti(const Matrix &delta, const Matrix &theta);
    void GradThetaMulti(const Matrix &delta_T, const Matrix &output_T);

    Matrix & operator /= (const double &k);

    friend ostream & operator << (ostream &os, const Matrix &matrix);
    Matrix Transposition() const;

    void Map(double (*func)(const double &x));
    void clear(){memset(value_, 0, sizeof(double) * row_number_ * col_number_);}
    void random();

    void Save(FILE *fp);
    void Load(FILE *fp);
};

class ColVector;
class RowVector;

class ColVector : public Matrix{
private :

public :
    ColVector() : Matrix(){}
    ColVector(int row_number) : Matrix(row_number, 1){}
    ColVector(int row_number, const double *value) :
        Matrix(row_number, 1, value){}
    ColVector(const Matrix &matrix) :
        Matrix(matrix . Row() * matrix . Col(), 1, matrix . ValueAddress()){}

    virtual ~ColVector(){}

    virtual double &Value(int i){
        return Matrix :: Value(i, 0);
    }
    virtual const double &Value(int i) const{
        return Matrix :: Value(i, 0);
    }

    RowVector Transposition() const;
};

class RowVector : public Matrix{
private :

public :
    RowVector() : Matrix(){}
    RowVector(int col_number) : Matrix(1, col_number){}
    RowVector(int col_number, const double *value) :
        Matrix(1, col_number, value){}
    RowVector(const Matrix &matrix) :
        Matrix(1, matrix . Row() * matrix . Col(), matrix . ValueAddress()){}

    virtual ~RowVector(){}

    virtual double &Value(int i){
        return Matrix :: Value(0, i);
    }
    virtual const double &Value(int i) const{
        return Matrix :: Value(0, i);
    }

    ColVector Transposition() const;
};

#endif
