#ifndef LAYER_H
#define LAYER_H

#include "matrix.h"

class Layer{
    private :
        int node_number_;
        ColVector input_, output_;
        RowVector delta_;
        
        double (*ActiveFunc_)(const double &x);
        double (*D_ActiveFunc_)(const double &x, const double &y);
        
        void Multi_D_ActiveFunc_();
        
    public :
        Layer(const int node_number = 0,
              double (*ActiveFunc)(const double &x) = nullptr,
              double (*D_ActiveFunc)(const double &x, const double &y) = nullptr);
        
        ~Layer(){}
        
        void InitForward(const double *input){output_ = Matrix(node_number_, 1, input);}        
        void Forward(const Layer *pre, const Matrix &theta, const ColVector &bias);
        
        void InitBackward(const double *delta);        
        void Backward(const Layer *next, const Matrix &theta, const ColVector &bias,
                      Matrix &grad_theta, ColVector &grad_bias);
        
        ColVector Output(){return output_;}
};

#endif