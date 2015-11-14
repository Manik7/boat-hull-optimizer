#ifndef BEZIER_QUADRATIC_H
#define BEZIER_QUADRATIC_H

#include <math>

#include "Point_3.h"

class Bezier_quadratic {

	Point_3 p1, p2, p3;
	Bezier_quadratic() {}; // private default constructor

public:
	Bezier_quadratic(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {};

	Point_3 point_at_t(double t) const;

	/* This function finds the first point along the bezier curve which has the specified z-coordinate value. Note that this only works if the given bezier spline is a monotonic function*/
	Point_3 find_point_along_z(double z_value, double epsilon = 0.001) const;

private:

	// Finds say the x-value of the bezier curve by specifiying t, and giving the x coordinates of the three points of this spline. Care must be taken to specify the values along the correct axis.
	double evaluate_1D(double t, double p1_coord, double p2_coord, double p3_coord) const;
	double evaluate_x(double t) const;
	double evaluate_y(double t) const;
	double evaluate_z(double t) const;

	// Do a binary-search over t, looking for the value which corresponds to the specified z-coordinate
	double find_t_for_z_coord(double z_value, double t_low, double t_high, double epsilon) const;
};

#endif // BEZIER_QUADRATIC_H
