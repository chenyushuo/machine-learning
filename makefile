objects = main.o net.o layer.o matrix.o function.o

all : main
	reset && ./main -i data.in -o data.out -m TRAINING --node=2

main : $(objects)
	g++ -o main $(objects) -Wall --std=c++11 -g

main.o : main.cpp matrix.h
	g++ -c main.cpp -Wall -DMAIN_CPP --std=c++11 -g

matrix.o : matrix.h matrix.cpp
	g++ -c matrix.cpp -Wall -DMATRIX_CPP --std=c++11 -g

layer.o : layer.h layer.cpp matrix.h
	g++ -c layer.cpp -Wall -DLAYER_CPP --std=c++11 -g

net.o : net.h net.cpp matrix.h layer.h function.h
	g++ -c net.cpp -Wall -DNET_CPP --std=c++11 -g

function.o : function.h function.cpp
	g++ -c function.cpp -Wall -DFUNCTION_CPP --std=c++11 -g


clean :
	rm $(objects) main -rf