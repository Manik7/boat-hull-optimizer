#ifndef HULL_H
#define HULL_H

#include <vector>

#include "Station.h"

class Hull {
	std::vector<Station> stations;

public:
	void generate_stations(); /*station_origin, std::string name, Bbox bbox, area, perimeter*/


//	Hull();
	double volume();
	double wetted_surface_area();
	double calculate_pitching_moment();
	

};

#endif // HULL_H
