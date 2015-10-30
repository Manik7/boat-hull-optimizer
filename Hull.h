#ifndef HULL_H
#define HULL_H

#include <vector>

#include "Station.h"
#include "Bbox.h"
#include "Constraints.h"

class Hull {
	std::vector<Station> stations;
	const double max_twist_deg = 20.0; // degrees per 1000 mm
	const int half_lwl;
	const int half_bwl;
	const int number_of_stations;

	double volume = 0.0;
	double wetted_surface_area = 0.0;
	double pitching_moment = 0.0;


public:
	Hull(int half_lwl = 3300, int half_bwl = 275, int number_of_stations = 2);


	

	void generate_stations(/*Bbox bbox, Constraints con*/); //TODO: You need a set of these, one pair for each station...


	
	void compute_properties();
	

};

#endif // HULL_H
