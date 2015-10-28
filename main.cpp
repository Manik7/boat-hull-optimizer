#include "Station.h"

int main () {
	Station station;
	double best_ap_ratio = 0.0;

	unsigned int iterations = 0;;
	unsigned const int number_of_iterations = 10000;
	//unsigned int number_of_solutions = 0;

	Station::line_print_labels();

	Bbox bbox(Point_3(0,0,0),Point_3(300,450,0));
	Constraints con(65000, 70000, 0.0, 25.0, 0, 60.0);
//	Constraints con(50000.0,70000.0,0.0,45.0,0.0,60.0);

	while(iterations<number_of_iterations) {
		station = Station(bbox, con);

		if(station.area_perimeter_ratio() > best_ap_ratio) {
			best_ap_ratio = station.area_perimeter_ratio();
			station.line_print();
		}
		iterations++;
	}
	//std::cout << number_of_solutions << " viable solutions found in " << number_of_iterations << " iterations." << std::endl;
}
