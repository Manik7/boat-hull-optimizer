PROJECT(hull_optimizer)

SET(CMAKE_CXX_FLAGS "-std=c++14 -O3")
# SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -g -pg -O0 ")
SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -g -pg -O0 -D DETERMINISTIC_RUN")

SET(hull_optimizer_SOURCES 
	Bezier_quadratic.cpp
	GreedyOptimizer.h
	Hull.cpp
	Main.cpp
	MonteCarloOptimizer.h
	OptimizableHull.cpp
	Station.cpp
	src_new/HillClimber.h
	src_new/HullModel.cpp
	src_new/OptimizableModel.cpp
# 	src_new/StationCalculator.cpp
)

ADD_EXECUTABLE(hull_optimizer ${hull_optimizer_SOURCES})
