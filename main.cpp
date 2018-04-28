#include "matrix.h"

#include <bits/stdc++.h>

using namespace std;

int main(){
    double a[6] = {1, 1, 0, 1, 1, 0};
    double b[2] = {2, 3};
    Matrix A(3, 2, a);
    Matrix B(2, 1, b);
    Matrix C = A * B;
    cout << C << endl;
}
