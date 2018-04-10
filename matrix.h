#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
    private :
        int col_number_, row_number_;
        double *value_;
        
        double &Value(int i, int j){
            return value_[i * col_number_ + j];
        }
        const double &Value(int i, int j) const{
            return value_[i * col_number_ + j];
        }
        
    public :
        Matrix(const int &col_number = 0, const int &row_number = 0, double *value = nullptr);
        Matrix(const Matrix &matrix);
        
        Matrix & operator = (const Matrix &matrix);
        
        ~Matrix();
};

#endif
