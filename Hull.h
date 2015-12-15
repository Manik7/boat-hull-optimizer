#ifndef HULL_H
#define HULL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Station.h"
#include "Bbox.h"
#include "Constraints.h"
#include "WaterlineCurve.h"

class Hull {

protected:
	typedef Bezier_quadratic Bezier;

	std::vector<Station> stations;
	const WaterlineCurve wl_curve;
	const double max_twist_deg = 20.0; // degrees per 1000 mm
	const double min_hull_volume = 0.65; //in cubic meters
	const int half_lwl;
	const int half_bwl;
	const int number_of_stations;

	double volume = 0.0;
	double wetted_surface_area = 0.0;
	double pitching_moment = 0.0;

public:
	Hull(int half_lwl = 3500, int half_bwl = 235, int number_of_stations = 5);

	void generate_stations();
	void compute_properties();
	void print_hull() const;
	void export_hull_coordinates(std::string filename = "example.dat") const;

private:
	void generate_optimized_station(Bbox& bbox, Constraints& con);
	
};

#endif // HULL_H
