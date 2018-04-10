matrix.o : matrix.h matrix.cpp
	g++ -c matrix.cpp -Wall -DMATRIX_CPP --std=c++11 -g
