CXX=g++
CFLAGS =-I.
ROOTCFLAGS := $(shell  $(ROOTSYS)/bin/root-config --cflags --libs)
OBJ = adsleyse.cpp


adsleyse :  $(OBJ)
	$(CXX) -o $@ $^ $(ROOTCFLAGS) $(CFLAGS)

clean:
	rm -f *.o $(objects) adsleyse