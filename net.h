#ifndef NET_H
#define NET_H

#include "matrix.h"
#include "layer.h"

class Net{
    private :
        static const char *save_file_name_ = "argc.log";
        static double learning_rate_ = 1.0;
        static double epslion_ = 1E-5;
        static long long recursion_times_ = -1;
        
        int layer_number_;
        Layer **layer_;
        
        Matrix **theta_, **grad_theta_;
        ColVector **bias_, **grad_bias_;
        
        int data_number_, feature_number_, category_number_;
        double **feature_, **category_;
        
        double Cost();
        
    public :
        Net(const int &layer_number, int *node_number, char *input_file_name);
        
        ~Net();
        
        static SetLearningRate(const double &learning_rate){learning_rate_ = learning_rate;}
        static SetEpslion(const double &epslion){epslion_ = epslion;}
        static SetRecursionTimes(const double &recursion_times){recursion_times_ = recursion_times;}
        
        void Training();
        void Save();
        void Load();
        void Testing();
};

#endif
