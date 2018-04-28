#include "net.h"

#include <cstdio>
#include <cstring>
#include <cmath>

#include "layer.h"
#include "matrix.h"
#include "function.h"

const char Net :: *save_file_name_;

Net :: Net(const int &layer_number, int *node_number, char *input_file_name) :
    layer_number_(layer_number), layer_(new *Layer[layer_number]),
    theta_(new *Matrix[layer_number]), grad_theta_(new *Matrix[layer_number]),
    bias_(new *ColVector[layer_number]), grad_bias_(new *ColVector[layer_number])
{
    
    FILE *fp = fopen(input_file_name, "r");
    if (fp == NULL){
        fprintf("please give a vaild file name\n");
        exit(1);
    }
    
    fscanf(fp, "%d%d%d", &data_number_, &feature_number_, &category_number_);
    feature_ = new *double[date_number_];
    category_ = new *double[date_number_];
    
    for (int i = 0; i < date_number_; i ++){
        feature_[i] = new double[feature_number_];
        for (int j = 0; j < feature_number_; j ++)
            fscanf(fp, "%d", &feature_[i][j]);
        
        category_[i] = new double[category_number_];
        memset(category_[i], 0, sizeof(double) * category_number_);
        int category;
        fscanf(fp, "%d", &category);
        category_[i][category] = 1.0;
    }
    
    node_number[0] = feature_number_;
    node_number[layer_number - 1] = category_number_;
    
    for (int i = 0; i < layer_number; i ++)
        layer_[i] = new Layer(node_number[i], &sigmoid, &D_sigmoid);
    
    for (int i = 0; i < layer_number - 1; i ++){
        theta_[i] = new Matrix(node_number[i + 1], node_number[i]);
        grad_theta_[i] = new Matrix(node_number[i + 1], node_number[i]);
        bias_[i] = new ColVector(node_number[i + 1]);
        grad_bias_[i] = new ColVector(node_number[i + 1]);
    }
}

Net :: ~Net(){
    for (int i = 0; i < layer_number; i ++)
        delete layer_[i];
    
    for (int i = 0; i < layer_number - 1; i ++){
        delete theta_[i];
        delete grad_theta_[i];
        delete bias_[i];
        delete grad_bias_[i];
    }
    
    for (int i = 0; i < data_number_; i ++){
        delete [] feature_[i];
        delete [] category_[i];
    }
    
    delete [] layer_;
    delete [] theta_;
    delete [] grad_theta_;
    delete [] bias_;
    delete [] grad_bias_;
    delete [] feature_;
    delete [] category_;
}

double Net :: Cost(){
    double result = 0;
    for (int i = 0; i < layer_number_; i ++){
        grad_theta_ -> clear();
        grad_bias_ -> clear();
    }
    
    double *delta = new double[category_number_];
    for (int i = 0; i < date_number_; i ++){
        layer_[0] -> InitForward(feature_[i]);
        for (int j = 1; j < layer_number_; j ++)
            layer_[j] -> Forward(layer[j - 1], *theta_[j - 1], *bias[j - 1]);
        
        ColVector output = layer[layer_number_ - 1] -> Output();
        for (int k = 0; k < category_number_; k ++){
            double out = output . Value(k), expect = category_number_[i][k];
            double cost = CostFunc(out, expect);
            result += cost;
            delta[k] = D_CostFunc(out, expect, cost);
        }
        
        layer_[layer_number_ - 1] -> InitBackward(delta);
        for (int j = layer_number_ - 2; j >= 0; j ++)
            layer_[j] -> Backward(layer_[j + 1], *theta_[j], *bias_[j],
                                  *grad_theta_[j], *grad_bias_[j]);
    }
    
    result /= date_number_;
    for (int j = 0; j < layer_number_ - 1; j ++){
        *grad_theta_[j] /= date_number_;
        *grad_bias_[j] /= date_number_;
    }
    
    for (int j = 0; j < layer_number_ - 1; j ++){
        *theta_[j] -= learning_rate_ * grad_theta_[j];
        *bias_[j] -= learning_rate * grad_bias_[j];
    }
    
    delete [] delta;
}

void Net :: Training(){
    for (int i = 0; i < layer_number_; i ++){
        theta_[i] -> random();
        bias_[i] -> random();
    }
    
    double new_cost = Cost(), old_cost;
    int recursion_times = 0;
    do{
        recursion_times ++;
        old_cost = new_cost;
        new_cost = Cost();
    }while (fabs(old_cost - new_cost) >= epslion_ && recursion_times != recursion_times_);
    
    Save();
}

void Net :: Save(){
    FILE *fp = fopen(save_file_name_, "wb");
    
}
