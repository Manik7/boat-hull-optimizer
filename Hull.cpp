#include "Hull.h"

Hull::Hull(int half_lwl, int half_bwl, int number_of_stations) : half_lwl(half_lwl), half_bwl(half_bwl), number_of_stations(number_of_stations) {
	
}

void Hull::generate_stations() {

	Station station;
	double best_ap_ratio = 0.0;
	unsigned int iterations = 0;
	unsigned const int number_of_iterations = 10000;

	Station::line_print_labels();

	for (int i = 0; i<2; i++) {

		best_ap_ratio = 0.0;
		iterations = 0;
		double station_spacing = half_lwl/(number_of_stations-1);

		Bbox bbox(Point_3(0,0,i*station_spacing),Point_3(300,450,i*station_spacing));
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
		std::cout << std::endl;
	}
	
}



//You could assert that a hull must have at least two stations for any of the calculations to work

void Hull::compute_properties() {
	for (const auto& it : stations) {
		//TODO
	}
	
}
