#include "OptimizableHullFactory.h"

OptimizableHull OptimizableHullFactory::classicGenerator() {
	OptimizableHull hull;

	generate_stations(hull);
	hull.compute_properties();
	
	return hull;
}

void OptimizableHullFactory::generate_optimized_station(Station& station, Bbox& bbox, Constraints& con) {

	Station working_station;
	double best_ap_ratio = 0.0;
	unsigned int iterations = 0;
	const unsigned int number_of_iterations = 1000;

	while(iterations<number_of_iterations) {
		working_station = Station(bbox, con);

		if(working_station.area_perimeter_ratio() > best_ap_ratio) {
			best_ap_ratio = working_station.area_perimeter_ratio();
			station = working_station;		
		}
		iterations++;
	}
}