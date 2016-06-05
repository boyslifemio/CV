CFLAGS = -O2 `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv` -lstdc++ -lm -std=c++11

track: track.cpp sub.hpp
	gcc $(CFLAGS) track.cpp -o backup.out $(LIBS)


clean:
	rm *.out
