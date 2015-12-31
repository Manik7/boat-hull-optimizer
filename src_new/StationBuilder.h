#ifndef STATION_BUILDER_H
#define STATION_BUILDER_H

#include <cmath>

#include "StationProperties.h"

template<typename T>
struct StationBuilder {
	
	StationProperties constructStation (T beam_x, T chine_x, T chine_y, T keel_y);

private:
	inline double sq_edge_length(int a_x, int a_y, int b_x, int b_y);
	inline double deg(double rad);
};



#endif // STATION_BUILDER_H
