#include "layer.h"

#include <cstdio>
#include <cstring>

#include "matrix.h"

using namespace std;

Layer :: Layer(const int node_number,
               double (*ActiveFunc)(const double &x),
               double (*D_ActiveFunc)(const double &x, const double &y)) :
    node_number_(node_number),
    input_(node_number_), output_(node_number_),
    delta_(node_number_),
    ActiveFunc_(ActiveFunc), D_ActiveFunc_(D_ActiveFunc)
{

}

void Layer :: Forward(const Layer *pre,
                      const Matrix &theta, const ColVector &bias)
{
    //input_ = theta * pre -> output_;
    input_.InputMulti(theta, pre -> output_);
    input_ += bias;
    output_ = input_;
    output_ . Map(ActiveFunc_);
}

inline void Layer :: Multi_D_ActiveFunc_(){
    for (int i = 0; i < node_number_; i ++)
        delta_ . Value(i) *= D_ActiveFunc_(input_ . Value(i), output_ . Value(i));
}

void Layer :: InitBackward(const double *delta){
    delta_ = RowVector(node_number_, delta);
//    Multi_D_ActiveFunc_();
}

void Layer :: Backward(const Layer *next, const Matrix &theta,
                       Matrix &grad_theta, ColVector &grad_bias)
{
    //delta_ = next -> delta_ * theta;
    delta_.DeltaMulti(next -> delta_, theta);
    Multi_D_ActiveFunc_();

    ColVector delta_T = next -> delta_.Transposition();
    RowVector output_T = output_.Transposition();
    //grad_theta += delta_T * output_T;
    grad_theta.GradThetaMulti(delta_T, output_T);
    grad_bias += delta_T;
}

void Layer :: Backward_zero(const Layer *next, const Matrix &theta,
                            Matrix &grad_theta, ColVector &grad_bias)
{
    ColVector delta_T = next -> delta_.Transposition();
    RowVector output_T = output_.Transposition();
    //grad_theta += delta_T * output_T;
    grad_theta.GradThetaMulti(delta_T, output_T);
    grad_bias += delta_T;
}