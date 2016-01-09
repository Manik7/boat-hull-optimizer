PROJECT(hull_optimizer)

SET(CMAKE_CXX_FLAGS "-std=c++14 -DNDEBUG -O3")
# SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -pg")
SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -pg -D DETERMINISTIC_RUN")

SET(hull_optimizer_SOURCES 
	Bezier_quadratic.cpp
	GreedyOptimizer.h
	Hull.cpp
	Main.cpp
	MonteCarloOptimizer.h
	OptimizableHull.cpp
	Station.cpp
	src_new/HillClimber.h
# 	src_new/HullModel.tpp
# 	src_new/OptimizableModel.cpp
# 	src_new/StationCalculator.cpp
)

ADD_EXECUTABLE(hull_optimizer ${hull_optimizer_SOURCES})
