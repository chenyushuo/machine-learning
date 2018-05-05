#ifndef FUNCTION_H
#define FUNCTION_H

double sigmoid(const double &x);
double D_sigmoid(const double &x, const double &y);

double ReLU(const double &x);
double D_ReLU(const double &x, const double &y);

double CostFunc(const double &x1, const double &x2);
double D_CostFunc(const double &x1, const double &x2, const double &y);

#endif
