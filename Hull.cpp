#include "Hull.h"


void Hull::generate_stations() {

	Station station;
	double best_ap_ratio = 0.0;
	unsigned int iterations = 0;
	unsigned const int number_of_iterations = 10000;

	Station::line_print_labels();

	for (int i = 0; i<1; i++) {

		best_ap_ratio = 0.0;
		iterations = 0;

		Bbox bbox(Point_3(0,0,0),Point_3(300,450,0));
		Constraints con(65000, 70000, 0.0, 25.0, 0, 60.0);

		while(iterations<number_of_iterations) {
			station = Station(bbox, con);

			if(station.area_perimeter_ratio() > best_ap_ratio) {
				best_ap_ratio = station.area_perimeter_ratio();
				station.line_print();
			}
			iterations++;
		}
		stations.push_back(station);
	}
	
}

/*Hull::Hull() {
	
}*/

//You could assert that a hull must have at least two stations for any of the calculations to work

double Hull::volume() { //TODO
	return 0.0;
}

double Hull::wetted_surface_area() { //TODO
	return 0.0;
}

double Hull::calculate_pitching_moment() { //TODO
	return 0.0;
}
