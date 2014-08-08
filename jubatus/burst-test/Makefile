document-generator: document_generator.o
	$(CXX) -std=gnu++11 -o $@ $< `pkg-config jubatus --libs`

.cpp.o:
	$(CXX) -c -std=gnu++11 -Wno-deprecated-declarations $< `pkg-config jubatus --cflags`
