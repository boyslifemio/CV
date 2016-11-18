CFLAGS = -O2 `pkg-config --cflags opencv` 
LIBS = `pkg-config --libs opencv` -lglog -lstdc++ -std=c++11 

all : calibration

calibration : calibration.cc
	g++ $(CFLAGS) calibration.cc -o calibration.out $(LIBS)

clean:
	rm *.out

