#include "function.h"

#include <cmath>

using namespace std;

inline static double sqr(const double &x){return x * x;}

double sigmoid(const double &x){
    return 1.0 / (1.0 + exp(- x));
}

double D_sigmoid(const double &x, const double &y){
    return y * (1.0 - y);
}

static const double alpha = 0.01;

double ReLU(const double &x){
    return (x >= 0) ? x : alpha * x;
}

double D_ReLU(const double &x, const double &y){
    return (x >= 0) ? 1 : alpha;
}

double CostFunc(const double &x1, const double &x2){
    return sqr(x1 - x2) / 2;
}

double D_CostFunc(const double &x1, const double &x2, const double &y){
    return (x1 - x2);
}
