#ifndef HULL_H
#define HULL_H

#include <vector>
#include <list>

#include "Station.h"
#include "Bbox.h"
#include "Constraints.h"
#include "WaterlineCurve.h"

class Hull {
	typedef Bezier_quadratic Bezier;

	std::list<Station> stations;
	const WaterlineCurve wl_curve;
	const double max_twist_deg = 20.0; // degrees per 1000 mm
	const int half_lwl;
	const int half_bwl;
	const int number_of_stations;

	double volume = 0.0;
	double wetted_surface_area = 0.0;
	double pitching_moment = 0.0;

public:
	Hull(int half_lwl = 3500, int half_bwl = 290, int number_of_stations = 10);

	void generate_stations(/*Bbox bbox, Constraints con*/); //TODO: You need a list of these bbox-constraint-tuples, one tuple for each station

	void compute_properties();

private:
	void generate_optimized_station(Bbox& bbox, Constraints& con);
	
};

#endif // HULL_H
