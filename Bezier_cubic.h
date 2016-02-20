#ifndef BEZIER_CUBIC_H
#define BEZIER_CUBIC_H

#include <cmath>
#include <cassert>

#include "Point_3.h"

class Bezier_cubic {

protected:
	Point_3 p0, p1, p2, p3;
	Bezier_cubic() {}; // private default constructor

public:
	Bezier_cubic(Point_3& p0, Point_3& p1, Point_3& p2, Point_3& p3);
	
	double x_coordinate(double t) const; //Computes the x-coordinate of the bezier curve at t. This is faster than constructing the entire point P(t).
	// Do a binary-search over t, looking for the value which corresponds to the specified z-coordinate
	
	double find_t_for_z_coord(double z_value, double t_low = 0.0, double t_high = 1.0, double epsilon = 0.001) const;
	
protected:
	// Finds say the x-value of the bezier curve by specifiying t, and giving the x coordinates of the three points of this spline. Care must be taken to specify the values along the correct axis.
	double evaluate_1D(double t, double p0_coord, double p1_coord, double p2_coord, double p3_coord) const;
};

#endif // BEZIER_CUBIC_H