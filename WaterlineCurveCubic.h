#ifndef WATERLINE_CURVE_CUBIC_H
#define WATERLINE_CURVE_CUBIC_H

#include "Bezier_cubic.h"
#include "Point_3.h"
#include "HelperFunctions.h"

struct WaterlineCurveCubic : public Bezier_cubic {
	WaterlineCurveCubic(int half_lwl, int half_beam) {
		p0 = Point_3(half_beam, 0, 0);
		p1 = Point_3(half_beam, 0, int(half_lwl*0.65));
		p2 = Point_3(half_beam, 0, int(half_lwl*0.65));
		p3 = Point_3(0, 0, half_lwl);
	}
	
	WaterlineCurveCubic(int half_lwl, int half_beam, double entry_half_angle_deg) {
		
		int intersection_coord = int(-half_beam/tan(helper::rad(entry_half_angle_deg))+half_lwl);
		
		p0 = Point_3(half_beam, 0, 0);
		p1 = Point_3(half_beam, 0, intersection_coord);
		p2 = Point_3(half_beam, 0, intersection_coord);
		p3 = Point_3(0, 0, half_lwl);
	}
};

#endif // WATERLINE_CURVE_CUBIC_H
