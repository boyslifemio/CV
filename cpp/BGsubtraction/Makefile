CFLAGS = -O2 `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv` -lstdc++ -lm -std=c++11

track: track.cpp sub.hpp
	gcc $(CFLAGS) track.cpp -o backup.out $(LIBS)

test: concat_test.cpp sub_test.hpp
	gcc $(CFLAGS) concat_test.cpp -o test.out $(LIBS)


clean:
	rm *.out
