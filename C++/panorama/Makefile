#CFLAGS = -g -O0 `pkg-config --cflags opencv` -I/usr/local/include/eigen3 -I/usr/include/atlas
CFLAGS = -O2 `pkg-config --cflags opencv` -I/usr/local/include/eigen3 -I/usr/include/atlas
#LIBS = `pkg-config --libs opencv` -lceres -lglog -ltbb -lgomp -llapack -latlas -lcholmod -lblas -lm
LIBS = `pkg-config --libs opencv` -lceres -lglog -lstdc++ -std=c++11

kadai2.out:panorama.cpp panorama.cpp
	g++ $(CFLAGS) panorama.cpp -I/usr/local/opt/opencv3/include -o panorama.out $(LIBS)

clean:
	rm -r panorama.out
