PROJECT(hull_optimizer)

SET(CMAKE_CXX_FLAGS "-std=c++11 -DNDEBUG -O3")
SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -pg -D DETERMINISTIC_RUN")

SET(hull_optimizer_SOURCES 
	Main.cpp
	#Bbox.h
	Bezier_quadratic.cpp
	GreedyOptimizer.h
	Hull.cpp
	MonteCarloOptimizer.h
	OptimizableHull.cpp
	#OptimizableHullFactory.cpp
	Station.cpp
	#WaterlineCurve.h
)

ADD_EXECUTABLE(hull_optimizer ${hull_optimizer_SOURCES})
