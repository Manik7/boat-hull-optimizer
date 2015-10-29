#ifndef HULL_H
#define HULL_H

#include <vector>

#include "Station.h"
#include "Bbox.h"
#include "Constraints.h"

class Hull {
	std::vector<Station> stations;

public:
	void generate_stations(/*Bbox bbox, Constraints con*/); //TODO: You need a set of these, one pair for each station...


//	Hull();
	double volume();
	double wetted_surface_area();
	double calculate_pitching_moment();
	

};

#endif // HULL_H
