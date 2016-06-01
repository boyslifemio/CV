epipolar.out:epipolar.cpp epipolar.cpp
	g++ epipolar.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11 -o epipolar.out

beta.out:epipolar_beta.cpp epipolar.hpp
	g++ epipolar_beta.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11 -o epipolar.out


clean:
	rm -r epipolar.out
