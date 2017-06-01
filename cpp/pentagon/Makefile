CFLAGS = -O2 `pkg-config --cflags opencv` 
LIBS = `pkg-config --libs opencv` -lglog -lstdc++ -std=c++11

pentagon: drawPentagon.cc
	g++ $(CFLAGS) drawPentagon.cc -o pentagon.out $(LIBS)

clean:
	rm *.out

