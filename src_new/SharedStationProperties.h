#ifndef SHARED_STATION_PROPERTIES_H
#define SHARED_STATION_PROPERTIES_H

#include <cmath>

template<typename CoordType>
struct SharedStationProperties {
	
	// attributes
	
	//TODO these could all be const since the properties object is recreated each time
	double area;
	double perimeter;
// 	double sq_perimeter;
	double flare_rad;
	double deadrise_rad;
	
	SharedStationProperties() : area(777), perimeter(777), flare_rad(777), deadrise_rad(777) {}
	
	SharedStationProperties(double area, double perimeter, double flare_rad, double deadrise_rad) : 
		area(area), perimeter(perimeter), flare_rad(flare_rad), deadrise_rad(deadrise_rad) {}
		
	SharedStationProperties(CoordType beam_x, CoordType chine_x, CoordType chine_y, CoordType keel_y) :
		area(chine_x * chine_y + ( (beam_x - chine_x) * chine_y + chine_x * (keel_y - chine_y) )/2.0),
		perimeter(edge_length(0, keel_y, chine_x, chine_y) + edge_length(chine_x, chine_y, beam_x, 0)),
// 		sq_perimeter(sq_edge_length(0, keel_y, chine_x, chine_y) + edge_length(chine_x, chine_y, beam_x, 0)),
		
		flare_rad(atan2(beam_x-chine_x, chine_y)),
		deadrise_rad(atan2(keel_y - chine_y, chine_x)) {}

private:
				
	inline double sq_edge_length(int a_x, int a_y, int b_x, int b_y) const {
		return (a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y);
	}
	
	inline double edge_length(int a_x, int a_y, int b_x, int b_y) const {
		return sqrt(sq_edge_length(a_x, a_y, b_x, b_y));
	}
};

#endif // SHARED_STATION_PROPERTIES_H