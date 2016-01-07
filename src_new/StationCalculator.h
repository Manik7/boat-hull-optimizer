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
	const HullParameters hull_properties; //TODO: make static
	
	// constructor
	StationCalculator(HullParameters& hullprop) : hull_properties(hullprop) {}
	
	// public methods	
	StationProperties calculate_station_properties(T beam_x, T chine_x, T chine_y, T keel_y) {
		double area(chine_x * chine_y + ( (beam_x - chine_x) * chine_y + chine_x * (keel_y - chine_y) )/2.0);
		double sq_perimeter(sq_edge_length(0, keel_y, chine_x, chine_y) + sq_edge_length(chine_x, chine_y, beam_x, 0));
		double flare_deg(deg(atan( (beam_x-chine_x) / chine_y )));
		double deadrise_deg(deg(atan( (keel_y - chine_y) / chine_x )));
		
		return StationProperties(area, sq_perimeter, flare_deg, deadrise_deg);
	}
	
private:
	inline double sq_edge_length(int a_x, int a_y, int b_x, int b_y) {
		return (a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y);
	}
	
	inline double deg(double rad) {
		return rad/3.14159265*180.0;
	}
};



#endif // STATION_CALCULATOR_H
