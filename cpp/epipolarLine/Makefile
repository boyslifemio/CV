epipolar :epipolar.cpp epipolar.hpp
	g++ epipolar.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11 -o epipolar.out

beta :epipolar_beta.cpp epipolar.hpp
	g++ epipolar_beta.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -lcuda -lopencv_gpu -std=c++11 -o beta.out

clean:
	rm -r *.out
