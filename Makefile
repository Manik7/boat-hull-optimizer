CXX=g++
CXXFLAGS=-std=c++11 -pg
#-D DETERMINISTIC_RUN

all: Main

Main: Main.o Hull.o Station.o Bezier_quadratic.o OptimizableHull.o
	$(CXX) $(CXXFLAGS) $^ -o ./main

Bezier: Bezier_quadratic.o Bezier_quad_unit.o
	$(CXX) $(CXXFLAGS) $^ -o ./bezier_test

Main.o: Main.cpp
	$(CXX) $(CXXFLAGS) -c $<

Hull.o: Hull.cpp Hull.h
	$(CXX) $(CXXFLAGS) -c $<

Station.o: Station.cpp Station.h Point_3.h Bbox.h
	$(CXX) $(CXXFLAGS) -c $<

Bezier_quadratic.o: Bezier_quadratic.cpp Bezier_quadratic.h Point_3.h
	$(CXX) $(CXXFLAGS) -c $<

Bezier_quad_unit.o: Bezier_quad_unit.cpp 
	$(CXX) $(CXXFLAGS) -c $<

OptimizableHull.o: OptimizableHull.cpp OptimizableHull.h Optimizable.h
	$(CXX) $(CXXFLAGS) -c $<

#GreedyOptimizer.o: GreedyOptimizer.cpp GreedyOptimizer.h
#	$(CXX) $(CXXFLAGS) -c $<

visualization: clean_data Main
	./main
	gnuplot visualization.gp

clean_data:
	rm -f data/*.dat data/*.svg
	
profile: Main
	./main
	gprof –p ./main gmon.out
	
plot:
	gnuplot visualization.gp
	
clean:
	rm -f main bezier_test *.o data/*.dat data/*.svg

#%.o: %.cpp %.h
#	$(CXX) $(CXXFLAGS) -c $<

# $@ The file name of the target of the rule.
# $< The name of the first dependency.
# $^ The names of all the dependencies
