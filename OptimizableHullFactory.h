#ifndef OPTIMIZABLE_HULL_FACTORY_H
#define OPTIMIZABLE_HULL_FACTORY_H

#include "OptimizableHull.h"
#include "Station.h"

class OptimizableHullFactory {
	void generate_optimized_station(Station& station, Bbox& bbox, StationParameters& con);
	
public:
	OptimizableHull classicGenerator();
	
	
	
};

#endif // OPTIMIZABLE_HULL_FACTORY_H