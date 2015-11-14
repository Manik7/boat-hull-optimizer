#include "Station.h"

Station::Station(Bbox& bbox, Constraints& constraints) : bbox(bbox), c(constraints), origin(bbox.min.x, bbox.min.y, bbox.min.z), beam(bbox.max.x, 0, origin.z, "beam") {
		generate_chine_and_keel();		
}

void Station::generate_chine_and_keel() {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<int> distr;
	bool foundSolution = false;

	chine = Point(0, 0, origin.z, "chine");

	for (int i = 0; i<number_of_iterations; i++) {

		distr = std::uniform_int_distribution<int>(resolution(bbox.max.x)/4, resolution(bbox.max.x)); // set to x range
		chine.x = distr(eng)*xy_resolution;  // random x_chine

		distr = std::uniform_int_distribution<int>(resolution(bbox.max.y)/4, resolution(bbox.max.y)); //set to y range
		chine.y = distr(eng)*xy_resolution; // random y_chine
		keel = Point(0, distr(eng)*xy_resolution, origin.z, "keel"); // random y_keel
		
		if (satisfies_constraints()) {
			foundSolution = true;
			break;
		}

	}
	if (!foundSolution) {
		std::cout << "Could not find a solution for section at z = " << origin.z << " after " << number_of_iterations << " iterations." << std::endl;
	}
}

int Station::resolution(int input) const {
	return input/xy_resolution;
}

//TODO: Performance could be improved by using the squared_edge_length to avoid taking roots
double Station::edge_length(Point a, Point b) const {
	return sqrt(pow(double(a.x - b.x), 2.0) + pow(double(a.y - b.y), 2.0));
}

int Station::sq_edge_length(Point a, Point b) const {
	return pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0);
}

double Station::area() const {
	return double(chine.x * chine.y 
		+ ( (beam.x - chine.x) * chine.y
		+ chine.x * (keel.y - chine.y) )/2);
}

double Station::perimeter() const {
	return edge_length(beam, chine) + edge_length(chine, keel);
}

int Station::sq_perimeter() const {
	return sq_edge_length(beam, chine) + sq_edge_length(chine, keel);
}

double Station::z_coord() const {
	return origin.z;
}

double Station::area_perimeter_ratio() const {
	return area()/perimeter();
}

double Station::area_sq_perimeter_ratio() const {
	return double(area())/double(sq_perimeter());
}

double Station::flare_angle_deg() const {
	return acos(double(chine.y)/edge_length(beam,chine)) / 3.1415 * 180.0;
}

double Station::deadrise_angle_deg() const {
	return acos(chine.x/edge_length(chine, keel)) / 3.1415 * 180.0;
}

bool Station::satisfies_constraints() const {
	if (area() > c.area_min && area() < c.area_max && flare_angle_deg() > c.flare_min && flare_angle_deg() < c.flare_max && deadrise_angle_deg() > c.deadrise_min && deadrise_angle_deg() < c.deadrise_max) { 
		return true;
	} else return false;
}

void Station::line_print_labels() {
	std::cout << "station\t" << "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "perim.\t" << "a/p\t" <<"flare\t" << "deadrise" << std::endl;
}

void Station::line_print() const {
	std::cout << origin.z << "\t" << chine.x << "\t" << chine.y << "\t" << keel.y 
		<< "\t" << area() << "\t" << perimeter() << "\t" << area_perimeter_ratio() << "\t"
		<<flare_angle_deg() << "\t" << deadrise_angle_deg() << std::endl;
}
