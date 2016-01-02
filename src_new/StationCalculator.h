#ifndef STATION_CALCULATOR_H
#define STATION_CALCULATOR_H

#include <cmath>
#include <cassert>

#include "StationProperties.h"
#include "HullParameters.h"
#include "../WaterlineCurve.h"

//TODO: Make the hull_properties into template parameters to clean the code up a bit (?)

template<typename T, typename HullParameters>
struct StationCalculator {
	
	// attributes
	static const HullParameters hull_properties;
	
	// constructor
	StationCalculator(HullParameters& hullprop);
	
	// public methods	
	StationProperties calculate_station_properties(T beam_x, T chine_x, T chine_y, T keel_y);
	
private:
	inline double sq_edge_length(int a_x, int a_y, int b_x, int b_y);
	inline double deg(double rad);
};



#endif // STATION_CALCULATOR_H
