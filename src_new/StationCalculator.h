#ifndef STATION_CALCULATOR_H
#define STATION_CALCULATOR_H

#include <cmath>
#include <cassert>

#include "StationProperties.h"
#include "HullParameters.h"
#include "../WaterlineCurve.h"

//TODO: Make the hull_parameters into template parameters to clean the code up a bit (?)

template<typename T, typename HullParameters>
struct StationCalculator {
	
	// attributes
	const HullParameters hull_parameters; //TODO: make static
	
	// constructor
	StationCalculator(HullParameters& hullparam) : hull_parameters(hullparam) {}
	
	// public methods	
	StationProperties calculate_station_properties(T beam_x, T chine_x, T chine_y, T keel_y) const {
		double area(chine_x * chine_y + ( (beam_x - chine_x) * chine_y + chine_x * (keel_y - chine_y) )/2.0);
		double perimeter(edge_length(0, keel_y, chine_x, chine_y) + edge_length(chine_x, chine_y, beam_x, 0));
// 		double sq_perimeter(sq_edge_length(0, keel_y, chine_x, chine_y) + edge_length(chine_x, chine_y, beam_x, 0));
		
// 		assert(chine_y>0);
// 		double flare_deg(deg(atan( (beam_x-chine_x) / chine_y )));
// 		double flare_deg((chine_y > 0) ? deg(atan( (beam_x-chine_x) / chine_y )) : 90.0);
		double flare_rad(atan2(beam_x-chine_x, chine_y));
		
// 		assert(chine_x>0);
// 		double deadrise_deg(deg(atan( (keel_y - chine_y) / chine_x )));		
// 		double deadrise_deg((chine_x > 0) ? deg(atan( (keel_y - chine_y) / chine_x )) : 0.0);
		double deadrise_rad(atan2(keel_y - chine_y, chine_x));
		
		return StationProperties(area, perimeter, flare_rad, deadrise_rad);
	}
	
private:
	inline double sq_edge_length(int a_x, int a_y, int b_x, int b_y) const {
		return (a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y);
	}
	
	inline double edge_length(int a_x, int a_y, int b_x, int b_y) const {
		return sqrt(sq_edge_length(a_x, a_y, b_x, b_y));
	}
};



#endif // STATION_CALCULATOR_H
