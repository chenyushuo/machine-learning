#include "net.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstddef>

#include <algorithm>

#include "layer.h"
#include "matrix.h"
#include "function.h"
#include "kbhit.h"

using namespace std;

const char * Net :: save_file_name_ = "argv.log";
double Net :: learning_rate_ = 1.0;
double Net :: epslion_ = 1E-9;
long long Net :: recursion_times_ = -1;
int Net :: block_size_ = 100;

inline void Net :: ReadData(const char *input_file_name){
    FILE *fp = fopen(input_file_name, "r");
    if (fp == NULL){
        fprintf(stderr, "please give a vaild input file name\n");
        exit(1);
    }

    fscanf(fp, "%d%d%d", &data_number_, &feature_number_, &category_number_);
    feature_ = new double *[data_number_];
    category_ = new double *[data_number_];

    int *trans = new int [data_number_];
    iota(trans, trans + data_number_, 0);
    if (status_ == TRAINING){
        random_shuffle(trans, trans + data_number_);
    }
    for (int i = 0; i < data_number_; i ++){
        double *f = new double[feature_number_];
        feature_[trans[i]] = f;
        for (int j = 0; j < feature_number_; j ++)
            fscanf(fp, "%lf", f + j);

        category_[trans[i]] = new double [category_number_]();
        int category;
        fscanf(fp, "%d", &category);
        category_[trans[i]][category] = 1.0;
    }
    delete [] trans;
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

Net :: Net(int layer_number, int *node_number,
           const char *input_file_name, const char *output_file_name) :
    status_(TRAINING), output_file_name_(output_file_name),
    layer_number_(layer_number), layer_(new Layer *[layer_number_]),
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

Net :: Net(int layer_number, const int *node_number,
           const char *output_file_name) :
    status_(TESTING), output_file_name_(output_file_name),
    layer_number_(layer_number), layer_(new Layer *[layer_number_]),
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

double Net :: TrainingCost(){
    double total_cost = 0;
    for (int i = 0; i < layer_number_ - 1; i ++){
        grad_theta_[i] -> clear();
        grad_bias_[i] -> clear();
    }

    double *delta = new double[category_number_];
    static int start = 0;
    int training_number = 0;
    for (int i = start; i - start < block_size_ && i < data_number_; i ++){
        training_number ++;
        layer_[0] -> InitForward(feature_[i]);
        for (int j = 1; j < layer_number_; j ++)
            layer_[j] -> Forward(layer_[j - 1], *theta_[j - 1], *bias_[j - 1]);

        const ColVector &output = layer_[layer_number_ - 1] -> Output();
        for (int k = 0; k < category_number_; k ++){
            double out = output . Value(k), expect = category_[i][k];
            double cost = CostFunc(out, expect);
            total_cost += cost;
            delta[k] = D_CostFunc(out, expect, cost);
        }

        layer_[layer_number_ - 1] -> InitBackward(delta);
        for (int j = layer_number_ - 2; j >= 1; j --){
            layer_[j] -> Backward(layer_[j + 1], *theta_[j],
                                  *grad_theta_[j], *grad_bias_[j]);
        }
        layer_[0] -> Backward_zero(layer_[1], *theta_[0],
                                   *grad_theta_[0], *grad_bias_[0]);
    }

    start += training_number;
    if (start == data_number_)
        start = 0;
    total_cost /= training_number;
    for (int j = 0; j < layer_number_ - 1; j ++){
        *grad_theta_[j] /= training_number;
        *grad_theta_[j] *= learning_rate_;
        *grad_bias_[j] /= training_number;
        *grad_bias_[j] *= learning_rate_;
    }

    for (int j = 0; j < layer_number_ - 1; j ++){
        *theta_[j] -= *grad_theta_[j];
        *bias_[j] -= *grad_bias_[j];
    }
    delete [] delta;
    return total_cost;
}

double Net :: TestingCost(){
    double total_cost = 0;
    for (int i = 0; i < data_number_; i ++){
        layer_[0] -> InitForward(feature_[i]);
        for (int j = 1; j < layer_number_; j ++)
            layer_[j] -> Forward(layer_[j - 1], *theta_[j - 1], *bias_[j - 1]);

        const ColVector &output = layer_[layer_number_ - 1] -> Output();
        for (int k = 0; k < category_number_; k ++){
            double out = output . Value(k), expect = category_[i][k];
            double cost = CostFunc(out, expect);
            total_cost += cost;
        }

        double Max = 0;
        int choise = -1;
        for (int k = 0; k < category_number_; k ++){
            if (Max < output . Value(k)){
                Max = output . Value(k);
                choise = k;
            }
        }
        FILE *fp = fopen(output_file_name_, "a+");
        fprintf(fp, "choice = %d, possibility = %.4f\n", choise, Max);
        fclose(fp);
    }

    total_cost /= data_number_;
    return total_cost;
}

void Net :: SetLearningRate(const double &learning_rate){
    if (learning_rate <= 0)
        return;
    learning_rate_ = learning_rate;
}

void Net :: SetEpslion(const double &epslion){
    if (epslion < 0)
        return;
    epslion_ = epslion;
}

void Net :: SetRecursionTimes(const double &recursion_times){
    if (recursion_times == 0)
        return;
    recursion_times_ = recursion_times;
}

void Net :: SetBlockSize(const int &size){
    if (size <= 0)
        return;
    block_size_ = size;
}

void Net :: Training(){
    for (int i = 0; i < layer_number_ - 1; i ++){
        theta_[i] -> random();
        bias_[i] -> random();
    }

    InitKeyboard();
    double new_cost = TrainingCost(), old_cost;
    int recursion_times = recursion_times_;
    int times = 0;
    do{
        recursion_times --;
        old_cost = new_cost;
        new_cost = TrainingCost();
        fprintf(stderr, "recursion times = %d, current cost = %f\n",
                ++ times, new_cost);
        if (CheckKeyboard())
            break;
    }while (fabs(old_cost - new_cost) >= epslion_ && recursion_times != 0);

    CloseKeyboard();
    FILE *fp = fopen(output_file_name_, "a+");
    fprintf(fp, "training cost = %f\n", TestingCost());
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
    double result = TestingCost();
    FILE *fp = fopen(output_file_name_, "a+");
    fprintf(fp, "testing cost = %f\n", result);
    fclose(fp);
    return result;
}
