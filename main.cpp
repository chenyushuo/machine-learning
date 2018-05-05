#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <ctime>

#include <string>

#include "net.h"

using namespace std;

void HelpMessage();
void SetArray(int &length_of_array, int * &array, const string &keys);

int main(int argc, char *argv[]){
    clock_t begin_time = clock();
    enum Net :: Status mode = Net :: TRAINING;
    string input_file_name, output_file_name;
    int layer_number = 0;
    int *node_number = nullptr;
    for (int i = 1; i < argc; i ++){
        string cur = argv[i], key;
        if (cur[0] == '-'){
            if (cur == "-m" || cur.find("--mode=") == 0){
                key = (cur == "-m") ? argv[++ i] : cur.substr(strlen("--mode="));

                if (key == "TRAINING")
                    mode = Net :: TRAINING;
                else if (key == "TESTING")
                    mode = Net :: TESTING;
                else{
                    fprintf(stderr, "mode must be 'TRAINING' or 'TESTING'!\n");
                    HelpMessage();
                }
            }
            else if (cur == "-i" || cur.find("--input=") == 0){
                key = (cur == "-i") ? argv[++ i] : cur.substr(strlen("--input="));
                input_file_name = key;
            }
            else if (cur == "-o" || cur.find("--output=") == 0){
                key = (cur == "-o") ? argv[++ i] : cur.substr(strlen("--output="));
                output_file_name = key;
            }
            else if (cur.find("--node=") == 0){
                key = cur.substr(strlen("--node="));
                SetArray(layer_number, node_number, key);
            }
            else if (cur.find("--learning_rate=") == 0){
                key = cur.substr(strlen("--learning_rate="));
                double val = -1;
                sscanf(key.c_str(), "%lf", &val);
                if (val <= 0){
                    fprintf(stderr, "learning rate must be a positive real number!\n");
                    HelpMessage();
                }
                Net :: SetLearningRate(val);
            }
            else if (cur.find("--epslion=") == 0){
                key = cur.substr(strlen("--epslion="));
                double val = -1;
                sscanf(key.c_str(), "%lf", &val);
                if (val < 0){
                    fprintf(stderr, "epslion must be a non-negative real number!\n");
                    HelpMessage();
                }
                Net :: SetEpslion(val);
            }
            else if (cur.find("--recursion_times=") == 0){
                key = cur.substr(strlen("--recursion_times="));
                int val = 0;
                sscanf(key.c_str(), "%d", &val);
                if (val == 0){
                    fprintf(stderr, "recursion times can't be zero!\n");
                    HelpMessage();
                }
                Net :: SetRecursionTimes(val);
            }
            else if (cur.find("--block_size=") == 0){
                key = cur.substr(strlen("--block_size="));
                int val = 0;
                sscanf(key.c_str(), "%d", &val);
                if (val <= 0){
                    fprintf(stderr, "block size must be positive number!\n");
                    HelpMessage();
                }
                Net :: SetBlockSize(val);
            }
            else
                HelpMessage();
        }
        else
            HelpMessage();
    }

    Net *net;
    if (mode == Net :: TRAINING){
        net = new Net(layer_number, node_number,
                      input_file_name.c_str(), output_file_name.c_str());
        net -> Training();
    }
    else{
        net = Net :: Load(output_file_name.c_str());
        net -> Testing(input_file_name.c_str());
    }
    fprintf(stderr, "tot time = %fs\n", 1.0 * (clock() - begin_time) / CLOCKS_PER_SEC);
    return 0;
}

static const int kLengthOfReadMe = 100000;
void HelpMessage(){
    FILE *fp = fopen("README.md", "r");
    if (fp == NULL){
        fprintf(stderr, "missing README.md!\n");
        exit(1);
    }
    char *message = new char [kLengthOfReadMe];
    fread(message, sizeof(char), kLengthOfReadMe, fp);
    fclose(fp);
    fprintf(stderr, "%s\n", message);
    delete [] message;
    exit(1);
}

void SetArray(int &length_of_array, int * &array, const string &keys){
    length_of_array = 3;
    for (auto ch : keys){
        if (ch == ',')
            length_of_array++;
        else if (!isdigit(ch)){
            fprintf(stderr, "node number must be interger!\n");
            HelpMessage();
        }
    }
    delete [] array;
    array = new int [length_of_array];
    int index = 1;
    string :: size_type begin = 0;
    string :: size_type end;
    do{
        end = keys.find(",", begin);
        if (end == string :: npos)
            end = keys.length();

        array[index ++] = atoi(keys.substr(begin, end - begin).c_str());
        begin = end + 1;
    }while (end != keys.length());
}
