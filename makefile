objects = main.o net.o layer.o matrix.o function.o kbhit.o

all : main
	reset && ./main -i data.in -o data.out -m TRAINING --node=2 --epslion=1E-10
	#reset && ./main -o ../minst/data.out --node=16,16 --epslion=1E-3 -m TRAINING -i ../minst/input.in

main : $(objects)
	g++ -o main $(objects) -Wall --std=c++11 -g -pg

main.o : main.cpp matrix.h
	g++ -c main.cpp -Wall -DMAIN_CPP --std=c++11 -g -pg

matrix.o : matrix.h matrix.cpp
	g++ -c matrix.cpp -Wall -DMATRIX_CPP --std=c++11 -g -pg

layer.o : layer.h layer.cpp matrix.h
	g++ -c layer.cpp -Wall -DLAYER_CPP --std=c++11 -g -pg

net.o : net.h net.cpp matrix.h layer.h function.h kbhit.h
	g++ -c net.cpp -Wall -DNET_CPP --std=c++11 -g -pg

function.o : function.h function.cpp
	g++ -c function.cpp -Wall -DFUNCTION_CPP --std=c++11 -g -pg

kbhit.o : kbhit.h kbhit.cpp
	g++ -c kbhit.cpp -Wall -DKBHIT_CPP --std=c++11 -g -pg


clean :
	rm $(objects) main -rf