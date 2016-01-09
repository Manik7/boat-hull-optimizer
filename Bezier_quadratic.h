#ifndef BEZIER_QUADRATIC_H
#define BEZIER_QUADRATIC_H

#include <cmath>

#include "Point_3.h"

//TODO: Write a unit test for this class

class Bezier_quadratic {
protected:
	Point_3 p0, p1, p2;
	Bezier_quadratic() {}; // private default constructor

public:
	Bezier_quadratic(Point_3& p0, Point_3& p1, Point_3& p2);

	double x_coordinate(double t) const; //Computes the x-coordinate of the bezier curve at t. This is faster than constructing the entire point P(t).
	double y_coordinate(double t) const;
	double z_coordinate(double t) const;
	Point_3 point_at_t(double t) const; // Constructs the point P(t) along the bezier curve

	/* This function finds the first point along the bezier curve which has the specified z-coordinate value. Note that this only works if the given bezier spline is a monotonic function*/
	Point_3 find_point_with_z_coord(double z_value, double epsilon = 0.001) const;

	// Do a binary-search over t, looking for the value which corresponds to the specified z-coordinate
	double find_t_for_z_coord(double z_value, double t_low = 0.0, double t_high = 1.0, double epsilon = 0.001) const;
	
protected:

	// Finds say the x-value of the bezier curve by specifiying t, and giving the x coordinates of the three points of this spline. Care must be taken to specify the values along the correct axis.
	double evaluate_1D(double t, double p0_coord, double p1_coord, double p2_coord) const;
	
	
};

#endif // BEZIER_QUADRATIC_H
