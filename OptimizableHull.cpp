#include "OptimizableHull.h"

OptimizableHull::OptimizableHull() : Optimizable(3*number_of_stations, 0, 99) {}

int OptimizableHull::get_parameter(int index) const {
	assert(index >= 0 && index < 3*number_of_stations);

	/*TODO: Understand what's going on with the next line. For some 
	 * reason if you set this to a Station& the code will not compile. 
	 * A Station& is not needed here since it's read-only access, so
	 * a copy is fine here, but it's weird that you can't get a reference
	 * here. In the set_parameter method below, where you really do need
	 * the reference, it's not a problem to get one.*/
	Station station = stations[index/3];
	double realParameterValue = station.get_parameter(index%3);
	Bbox bbox(station.bbox());

	if(index%3 == 0) { //TODO: an enum for the parameters would be nice
		return realParameterValue / bbox.max.x * maxValue;
	} 
	else if (index%3 == 1) {
		return realParameterValue / bbox.max.y * maxValue;
	} 
	else {
		return realParameterValue / bbox.max.y * maxValue;
	}
}

void OptimizableHull::set_parameter(int index, int value) {
	assert(index >= 0 && index < 3*number_of_stations);
	
	Station& station = stations[index/3];
	lastParameterIndex = index;
	lastParameterRealValue = station.get_parameter(index%3);
	int mappedParameterValue;
	Bbox bbox(station.bbox());

	if(index%3 == 0) { //TODO: an enum for the parameters would be nice
		mappedParameterValue = int(double(value) / double(maxValue) * double(bbox.max.x));
	} 
	else if (index%3 == 1) {
		mappedParameterValue = int(double(value) / double(maxValue) * double(bbox.max.y));
	} 
	else {
		mappedParameterValue = int(double(value) / double(maxValue) * double(bbox.max.y));
	}

	station.set_parameter(index%3, mappedParameterValue);
	parameterChanged = true;
	compute_properties();
}

void OptimizableHull::revert_last_change()
{
	if(parameterChanged) {
		stations[lastParameterIndex/3].set_parameter(lastParameterIndex%3, lastParameterRealValue);
		parameterChanged = false;
		compute_properties();
	}
}

bool OptimizableHull::satisfies_constraints() const {
	
	//TODO: Implement constraints for the entire hull such as the panel twist-rate and non-negative rocker
	
	if (this->volume < min_hull_volume) {
		return false;
	}
	
	for (auto station : stations) {
		if (!station.Station::satisfies_constraints()) return false;
	}

	return true; 
}

double OptimizableHull::fitness() { //TODO: proper fitness function needed here.
	return wetted_surface_area; // 1/WSA ensures that a lower WSA leads to increased fitness
}

void OptimizableHull::output()
{
	print_hull();
}

