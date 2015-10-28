#ifndef HULL_H
#define HULL_H

class Hull {
	std::vector<Station> stations;

	void generate_station(station_origin, std::string name, Bbox bbox, area, perimeter);

public:
	Hull();
	double volume();
	double wetted_surface_area();
	double calculate_pitching_moment();
	

};

#endif // HULL_H
