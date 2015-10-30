CXX=g++
CXXFLAGS=-std=c++11

all: Main

Main: Main.o Hull.o
	$(CXX) $(CXXFLAGS) $^ -o ./main

Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) -c $<

Hull.o: Hull.cpp Hull.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f main *.o

#%.o: %.cpp %.h
#	$(CXX) $(CXXFLAGS) -c $<




# $@ The file name of the target of the rule.
# $< The name of the first dependency.
# $^ The names of all the dependencies
