#include "Station.h"

Station::Station(Bbox& bbox, Constraints& constraints) : bbox(bbox), c(constraints), origin(bbox.min.x, bbox.min.y, bbox.min.z, "origin"), beam(bbox.max.x, 0, origin.z, "beam") {
		generate_chine_and_keel();		
}

void Station::generate_chine_and_keel() {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<int> distr;
	bool foundSolution = false;

	chine = Point(0, 0, origin.z, "chine"); //set the x-coordinate (beam) later

	for (int i = 0; i<number_of_iterations; i++) {

		distr = std::uniform_int_distribution<int>(resolution(bbox.max.x/4), resolution(bbox.max.x)); // set to x range, from 0 to bbox_max.x, instead of 0 I'm using 25%-of-beam instead, as solutions with the chine that far in would be discarded anyway
		chine.x = distr(eng)*xy_resolution;  // random x_chine

		distr = std::uniform_int_distribution<int>(resolution(bbox.max.y/4), resolution(bbox.max.y)); //set to y range, also using the 25% heuristic here
		chine.y = distr(eng)*xy_resolution; // random y_chine
		keel = Point(0, distr(eng)*xy_resolution, origin.z, "keel"); // random y_keel
		
		if (satisfies_constraints()) {
			foundSolution = true;
			break;
		}

	}
	if (!foundSolution) {
		//If you failed to find a solution, use this (very small) default station instead:
		chine.x = xy_resolution;
		chine.y = bbox.max.y/2;
		keel = Point(0, bbox.max.y, origin.z, "keel");
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
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "perim.\t" << "a/p\t" <<"flare\t" << "deadrise" << std::endl;
}

void Station::line_print() const {
	std::cout << origin.z << '\t' << beam.x << '\t' << chine.x << '\t' << chine.y << '\t' << keel.y 
		<< '\t' << area() << '\t' << perimeter() << '\t' << area_perimeter_ratio() << '\t'
		<<flare_angle_deg() << '\t' << deadrise_angle_deg() << std::endl;
}

int Station::getParameter(int index) {
	assert(index >= 0 && index < 3);
	
	switch (index) {
		case 0:
			return chine.x;
		case 1:
			return chine.y;
		case 2:
			return keel.y;
	}
}

void Station::setParameter(int index, int value) {
	assert(index >= 0 && index < 3);
	
	switch (index) {
		case 0:
			chine.x = value;
		case 1:
			chine.y = value;
		case 2:
			keel.y = value;
	}
}
