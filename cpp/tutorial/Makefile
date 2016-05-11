#CFLAGS = -g -O0 `pkg-config --cflags opencv` -I/usr/local/include/eigen3 -I/usr/include/atlas
CFLAGS = -O2 `pkg-config --cflags opencv`
#LIBS = `pkg-config --libs opencv` -lceres -lglog -ltbb -lgomp -llapack -latlas -lcholmod -lblas -lm
LIBS = `pkg-config --libs opencv` -lceres -lglog -lstdc++ -std=c++11

main: main.cpp match.hpp
	gcc $(CFLAGS) main.cpp -o main.out $(LIBS)

clean:
	rm *.out

