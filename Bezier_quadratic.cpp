#include "Bezier_quadratic.h"

Bezier_quadratic::Bezier_quadratic(Point_3& p0, Point_3& p1, Point_3& p2) : p0(p0), p1(p1), p2(p2) {};

Point_3 Bezier_quadratic::point_at_t(double t) const {
	Point_3 point;
	point.x = evaluate_x(t);
	point.y = evaluate_y(t);
	point.z = evaluate_z(t);
	point.name = "t=" + std::to_string(t);
	return point;
}

/* This function finds the first point along the bezier curve which has the specified z-coordinate value. Note that this only works if the given bezier spline is a monotonic function*/
Point_3 Bezier_quadratic::find_point_with_z_coord(double z_value, double epsilon) const {
	return point_at_t(find_t_for_z_coord(z_value, 0.0, 1.0, epsilon));
}

// Finds say the x-value of the bezier curve by specifiying t, and giving the x coordinates of the three points of this spline. Care must be taken to specify the values along the correct axis.
double Bezier_quadratic::evaluate_1D(double t, double p0_coord, double p1_coord, double p2_coord) const { 
	//TODO: assert 0 <= t <= 1
	return pow(1-t,2)*p0_coord + 2*(1-t)*t*p1_coord + pow(t,2)*p2_coord;
}

double Bezier_quadratic::evaluate_x(double t) const {
	return evaluate_1D(t, p0.x, p1.x, p2.x);
}

double Bezier_quadratic::evaluate_y(double t) const {
	return evaluate_1D(t, p0.y, p1.y, p2.y);
}

double Bezier_quadratic::evaluate_z(double t) const {
	return evaluate_1D(t, p0.z, p1.z, p2.z);
}

// Do a binary-search over t, looking for the value which corresponds to the specified z-coordinate
double Bezier_quadratic::find_t_for_z_coord(double z_value, double t_low, double t_high, double epsilon) const {
	double t = (t_low + t_high) / 2.0;
	double eval = evaluate_1D(t, p0.z, p1.z, p2.z);
	
	if (eval < z_value+epsilon && eval > z_value-epsilon) { //current t is just right
		return t;
	} else if (eval < z_value) { //current t too small, use a larger value
		return find_t_for_z_coord(z_value, t, t_high, epsilon);
	} else if (eval > z_value) { //current t too large, use a smaller value
		return find_t_for_z_coord(z_value, t_low, t, epsilon);
	}
}
