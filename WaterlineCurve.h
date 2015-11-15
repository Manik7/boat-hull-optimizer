#ifndef WATERLINE_CURVE_H
#define WATERLINE_CURVE_H

#include "Bezier_quadratic.h"
#include "Point_3.h"

struct WaterlineCurve : public Bezier_quadratic {
	WaterlineCurve(int half_lwl, int half_beam, double control_vertex_fraction = 0.65) {
		p0 = Point_3(half_beam, 0, 0);
		p1 = Point_3(half_beam, 0, int(half_lwl*control_vertex_fraction));
		p2 = Point_3(0, 0, half_lwl);
	}
};

#endif // WATERLINE_CURVE_H
