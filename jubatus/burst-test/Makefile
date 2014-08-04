burst-test: test.o
	$(CXX) -std=gnu++11 -o burst-test test.o `pkg-config jubatus --libs`

.cpp.o:
	$(CXX) -c -std=gnu++11 -Wno-deprecated-declarations $< `pkg-config jubatus --cflags`
