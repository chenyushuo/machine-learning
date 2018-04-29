#include "net.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstddef>

#include "layer.h"
#include "matrix.h"
#include "function.h"

using namespace std;

const char * Net :: save_file_name_ = "argv.log";
double Net :: learning_rate_ = 1.0;
double Net :: epslion_ = 1E-9;
long long Net :: recursion_times_ = -1;

inline void Net :: ReadData(const char *input_file_name){
    FILE *fp = fopen(input_file_name, "r");
    if (fp == NULL){
        fprintf(stderr, "please give a vaild input file name\n");
        exit(1);
    }

    fscanf(fp, "%d%d%d", &data_number_, &feature_number_, &category_number_);
    feature_ = new double *[feature_number_];
    category_ = new double *[category_number_];

    for (int i = 0; i < data_number_; i ++){
        feature_[i] = new double[feature_number_];
        for (int j = 0; j < feature_number_; j ++)
            fscanf(fp, "%lf", &feature_[i][j]);

        category_[i] = new double[category_number_]();
        int category;
        fscanf(fp, "%d", &category);
        category_[i][category] = 1.0;
    }
}

inline void Net :: NewLayer(const int *node_number){
    for (int i = 0; i < layer_number_; i ++)
        layer_[i] = new Layer(node_number[i], &sigmoid, &D_sigmoid);
}

inline void Net :: NewAgrc(const int *node_number){
    for (int i = 0; i < layer_number_ - 1; i ++){
        theta_[i] = new Matrix(node_number[i + 1], node_number[i]);
        bias_[i] = new ColVector(node_number[i + 1]);
    }
}

inline void Net :: NewGrad(const int *node_number){
    for (int i = 0; i < layer_number_ - 1; i ++){
        grad_theta_[i] = new Matrix(node_number[i + 1], node_number[i]);
        grad_bias_[i] = new ColVector(node_number[i + 1]);
    }
}

inline void Net :: ClearOutput(){
    FILE *fp = fopen(output_file_name_, "w");
    if (fp == NULL){
        fprintf(stderr, "please give a vaild output file name\n");
        exit(1);
    }
    fclose(fp);
}

Net :: Net(int layer_number_, int *node_number,
           const char *input_file_name, const char *output_file_name) :
           status_(TRAINING), output_file_name_(output_file_name),
           layer_number_(layer_number_), layer_(new Layer *[layer_number_]),
           theta_(new Matrix *[layer_number_]),
           grad_theta_(new Matrix *[layer_number_]),
           bias_(new ColVector *[layer_number_]),
           grad_bias_(new ColVector *[layer_number_])
{
    ReadData(input_file_name);
    ClearOutput();

    node_number[0] = feature_number_;
    node_number[layer_number_ - 1] = category_number_;

    NewLayer(node_number);
    NewAgrc(node_number);
    NewGrad(node_number);
}

Net :: Net(int layer_number_, const int *node_number,
           const char *output_file_name) :
           status_(TESTING), output_file_name_(output_file_name),
           layer_number_(layer_number_), layer_(new Layer *[layer_number_]),
           theta_(new Matrix *[layer_number_]), grad_theta_(nullptr),
           bias_(new ColVector *[layer_number_]), grad_bias_(nullptr)
{
    ClearOutput();
    NewLayer(node_number);
    NewAgrc(node_number);
}

Net :: ~Net(){
    for (int i = 0; i < layer_number_; i ++)
        delete layer_[i];

    for (int i = 0; i < layer_number_ - 1; i ++){
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
    double total_cost = 0;
    if (status_ == TRAINING){
        for (int i = 0; i < layer_number_ - 1; i ++){
            grad_theta_[i] -> clear();
            grad_bias_[i] -> clear();
        }
    }

    double *delta = new double[category_number_];
    for (int i = 0; i < data_number_; i ++){
        layer_[0] -> InitForward(feature_[i]);
        for (int j = 1; j < layer_number_; j ++)
            layer_[j] -> Forward(layer_[j - 1], *theta_[j - 1], *bias_[j - 1]);

        ColVector output = layer_[layer_number_ - 1] -> Output();
        for (int k = 0; k < category_number_; k ++){
            double out = output . Value(k), expect = category_[i][k];
            /*fprintf(stderr, "cases = %d, choise = %d, possibility = %f expect = %f\n",
                    i, k, out, expect);*/
            double cost = CostFunc(out, expect);
            total_cost += cost;
            delta[k] = D_CostFunc(out, expect, cost);
        }

        if (status_ == TRAINING){
            layer_[layer_number_ - 1] -> InitBackward(delta);
            for (int j = layer_number_ - 2; j >= 0; j --){
                layer_[j] -> Backward(layer_[j + 1], *theta_[j], *bias_[j],
                                      *grad_theta_[j], *grad_bias_[j]);
            }
        }
        else{
            double Max = 0;
            int choise = -1;
            for (int k = 0; k < category_number_; k ++){
                if (Max < output . Value(k)){
                    Max = output . Value(k);
                    choise = k;
                }
                /*fprintf(stderr, "cases = %d, choise = %d, \
                    possibility = %f expect = %f\n",
                    i, k, output . Value(k), category_[i][k]);*/
            }
            FILE *fp = fopen(output_file_name_, "a+");
            fprintf(fp, "choice = %d, possibility = %.4f\n", choise, Max);
            fclose(fp);
        }
    }

    total_cost /= data_number_;
    if (status_ == TRAINING){
        for (int j = 0; j < layer_number_ - 1; j ++){
            *grad_theta_[j] /= data_number_;
            *grad_bias_[j] /= data_number_;
        }

        for (int j = 0; j < layer_number_ - 1; j ++){
            *theta_[j] -= learning_rate_ * (*grad_theta_[j]);
            *bias_[j] -= learning_rate_ * (*grad_bias_[j]);
        }
    }
    delete [] delta;
    return total_cost;
}

inline void Net :: SetLearningRate(const double &learning_rate){
    learning_rate_ = learning_rate;
}

inline void Net :: SetEpslion(const double &epslion){
    epslion_ = epslion;
}

inline void Net :: SetRecursionTimes(const double &recursion_times){
    recursion_times_ = recursion_times;
}

void Net :: Training(){
    for (int i = 0; i < layer_number_ - 1; i ++){
        theta_[i] -> random();
        bias_[i] -> random();
    }

    double new_cost = Cost(), old_cost;
    int recursion_times = recursion_times_;
    do{
        recursion_times --;
        old_cost = new_cost;
        new_cost = Cost();
        fprintf(stderr, "current cost = %f\n", new_cost);
    }while (fabs(old_cost - new_cost) >= epslion_ && recursion_times != 0);

    FILE *fp = fopen(output_file_name_, "a+");
    fprintf(fp, "training cost = %f\n", Cost());
    fclose(fp);
    Save();
}

void Net :: Save(){
    FILE *fp = fopen(save_file_name_, "wb");
    fwrite(&layer_number_, sizeof(layer_number_), 1, fp);

    int *node_number = new int[layer_number_];
    for (int i = 0; i < layer_number_; i ++)
        node_number[i] = layer_[i] -> NodeNumber();
    fwrite(node_number, sizeof(int) * layer_number_, 1, fp);
    delete [] node_number;

    for (int i = 0; i < layer_number_ - 1; i ++){
        theta_[i] -> Save(fp);
        bias_[i] -> Save(fp);
    }

    fclose(fp);
}

Net * Net :: Load(const char *output_file_name){
    FILE *fp = fopen(save_file_name_, "rb");
    int layer_number;
    fread(&layer_number, sizeof(layer_number), 1, fp);

    int *node_number = new int[layer_number];
    fread(node_number, sizeof(int) * layer_number, 1, fp);

    Net *net = new Net(layer_number, node_number, output_file_name);

    for (int i = 0; i < layer_number - 1; i ++){
        net -> theta_[i] -> Load(fp);
        net -> bias_[i] -> Load(fp);
        // cerr << "theta : " << *(net -> theta_[i]) << endl;
        // cerr << "bias : " << *(net -> bias_[i]) << endl;
    }

    delete [] node_number;
    return net;
}

double Net :: Testing(const char *input_file_name){
    ReadData(input_file_name);
    double result = Cost();
    FILE *fp = fopen(output_file_name_, "a+");
    fprintf(fp, "testing cost = %f\n", result);
    fclose(fp);
    return result;
}