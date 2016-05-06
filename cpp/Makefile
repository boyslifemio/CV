CFLAGS = -O2 `pkg-config --cflags opencv` 
LIBS = `pkg-config --libs opencv` -lglog -lstdc++ -std=c++11

main: main.cpp
	g++ $(CFLAGS) main.cpp -o main.out $(LIBS)

optical: optical.cpp sub.hpp
	g++ $(CFLAGS) optical.cpp -o optical.out $(LIBS)

optmovie: optmovie.cpp sub.hpp
	g++ $(CFLAGS) optmovie.cpp -o optmovie.out $(LIBS)

version: version.cpp
	g++ $(CFLAGS) version.cpp -o version.out $(LIBS)

clean:
	rm *.out

