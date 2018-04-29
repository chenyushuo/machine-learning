#ifndef NET_H
#define NET_H

#include "matrix.h"
#include "layer.h"

class Net{
public:
    enum Status {TRAINING, TESTING};

private:
    const enum Status status_;
    const char *output_file_name_;
    static const char *save_file_name_;
    static double learning_rate_;
    static double epslion_;
    static long long recursion_times_;

    int layer_number_;
    Layer **layer_;

    Matrix **theta_, **grad_theta_;
    ColVector **bias_, **grad_bias_;

    int data_number_, feature_number_, category_number_;
    double **feature_, **category_;

    double Cost();

    void ReadData(const char *input_file_name);
    void NewLayer(const int *node_number);
    void NewAgrc(const int *node_number);
    void NewGrad(const int *node_number);
    void ClearOutput();
    void Save();

    Net(int layer_number, const int *node_number,
        const char *output_file_name);

public:
    Net(int layer_number, int *node_number,
        const char *input_file_name, const char *output_file_name);

    ~Net();

    static void SetLearningRate(const double &learning_rate);
    static void SetEpslion(const double &epslion);
    static void SetRecursionTimes(const double &recursion_times);

    void Training();
    static Net * Load(const char *output_file_name);
    double Testing(const char *input_file_name);
};

#endif
