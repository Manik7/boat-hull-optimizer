#include "OptimizableHull.cpp"

int OptimizableHull::get_parameter(int index) const {
	assert(index >= 0 && index < 3*number_of_stations);

	return stations[index/3].get_parameter(index%3);
}

void OptimizableHull::set_parameter(int index, int value) {
	assert(index >= 0 && index < 3*number_of_stations);

	stations[index/3].set_parameter(index%3, value);

}

bool OptimizableHull::satisfies_constraints() {
	return true; //TODO Implement
}

double OptimizableHull::fitness() { //TODO: proper fitness function needed here.
	compute_properties();
	return 1/wetted_surface_area; // 1/WSA ensures that a lower WSA leads to increased fitness
}
