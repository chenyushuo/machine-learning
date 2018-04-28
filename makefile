objects = main.o layer.o matrix.o

main : $(objects)
	g++ -o main $(objects) -Wall --std=c++11 -g

main.o : main.cpp matrix.h
	g++ -c main.cpp -Wall -DMAIN_CPP --std=c++11 -g

matrix.o : matrix.h matrix.cpp
	g++ -c matrix.cpp -Wall -DMATRIX_CPP --std=c++11 -g
	
layer.o : layer.h layer.cpp matrix.h
	g++ -c layer.cpp -Wall -DLAYER_CPP --std=c++11 -g

