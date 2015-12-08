#include "OptimizableHull.cpp"

int OptimizableHull::get_parameter(int index) const {
	assert(index >= 0 && index < 3*number_of_stations);

	Station st = stations[index/3];
	double realParameterValue = st.get_parameter(index%3);
	Bbox bbox = station.getBbox();

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

	Station st = stations[index/3];
	double mappedParameterValue;
	Bbox bbox = station.getBbox();

	if(index%3 == 0) { //TODO: an enum for the parameters would be nice
		mappedParameterValue = value / maxValue * bbox.max.x;
	} 
	else if (index%3 == 1) {
		mappedParameterValue = value / maxValue * bbox.max.y;
	} 
	else {
		mappedParameterValue = value / maxValue * bbox.max.y;
	}

	station.set_parameter(index%3, mappedParameterValue);

}

bool OptimizableHull::satisfies_constraints() {

	//TODO: Implement constraints for the entire hull, like the minimum volume and the panel twist-rate

	for (auto station : stations) {
		if !satisfies_constraints() return false;
	}

	return true; 
}

double OptimizableHull::fitness() { //TODO: proper fitness function needed here.
	compute_properties();
	return 1/wetted_surface_area; // 1/WSA ensures that a lower WSA leads to increased fitness
}
