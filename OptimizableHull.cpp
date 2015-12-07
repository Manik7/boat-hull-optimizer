#include "OptimizableHull.cpp"

int OptimizableHull::getParameter(int index) const {
	assert(index >= 0 && index < 3*number_of_stations);

	return stations[index/3].getParameter(index%3);
}

void OptimizableHull::setParameter(int index, int value) {
	assert(index >= 0 && index < 3*number_of_stations);

	stations[index/3].setParameter(index%3, value);

}

bool OptimizableHull::checkConstraints() {
	return true; //TODO Implement
}

double OptimizableHull::fitness() {
	compute_properties();
	return wetted_surface_area; //TODO: proper fitness function needed here
}
