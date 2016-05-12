CFLAGS = -O2 `pkg-config --cflags opencv` 
LIBS = `pkg-config --libs opencv` -lceres -lglog -lstdc++ -lm -std=c++11

optical: optical.cpp sub.hpp
	gcc $(CFLAGS) optical.cpp -o optical.out $(LIBS)

version: version.cpp
	gcc $(CFLAGS) version.cpp -o version.out $(LIBS)

clean:
	rm *.out

