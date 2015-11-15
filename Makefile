CXX=g++
CXXFLAGS=-std=c++11

all: Main

Main: Main.o Hull.o Station.o
	$(CXX) $(CXXFLAGS) $^ -o ./main

Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) -c $<

Hull.o: Hull.cpp Hull.h
	$(CXX) $(CXXFLAGS) -c $<

Station.o: Station.cpp Station.h Point_3.h Bbox.h
	$(CXX) $(CXXFLAGS) -c $<

Bezier: Bezier_quadratic.o Bezier_quad_unit.o
	$(CXX) $(CXXFLAGS) $^ -o ./bezier_test

Bezier_quad_unit.o: Bezier_quad_unit.cpp 
	$(CXX) $(CXXFLAGS) -c $<

Bezier_quadratic.o: Bezier_quadratic.cpp Bezier_quadratic.h Point_3.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f main bezier_test *.o

#%.o: %.cpp %.h
#	$(CXX) $(CXXFLAGS) -c $<




# $@ The file name of the target of the rule.
# $< The name of the first dependency.
# $^ The names of all the dependencies
