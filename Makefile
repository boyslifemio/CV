kadai.out:
	g++ kadai.cpp file.hpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -o kadai.out

clean:
	rm -r kadai.out
