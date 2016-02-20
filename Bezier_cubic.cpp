#include "Bezier_cubic.h"

Bezier_cubic::Bezier_cubic(Point_3& p0, Point_3& p1, Point_3& p2, Point_3& p3) : p0(p0), p1(p1), p2(p2), p3(p3) {};

double Bezier_cubic::x_coordinate(double t) const {
	return evaluate_1D(t, p0.x, p1.x, p2.x, p3.x);
}

double Bezier_cubic::find_t_for_z_coord(double z_value, double t_low, double t_high, double epsilon) const
{
	double t = (t_low + t_high) / 2.0;
	double eval = evaluate_1D(t, p0.z, p1.z, p2.z, p3.z);
	
	if (eval < z_value+epsilon && eval > z_value-epsilon) { //current t is just right
		return t;
	} else if (eval < z_value) { //current t too small, use a larger value
		return find_t_for_z_coord(z_value, t, t_high, epsilon);
	} else if (eval > z_value) { //current t too large, use a smaller value
		return find_t_for_z_coord(z_value, t_low, t, epsilon);
	}
}

double Bezier_cubic::evaluate_1D(double t, double p0_coord, double p1_coord, double p2_coord, double p3_coord) const
{
	assert(0 <= t && t <= 1);
	
	/* Evaluation with Px = (1-t)^3(P1x) + 3t(1-t)^2(P2x) + 3t^2(1-t)(P3x) + t^3(P4x)
	 * see http://stackoverflow.com/questions/7942840/bezier-curve-evaluation for the 
	 * formula. 
	 * 
	 * TODO: Use De Casteljau's algorithm instead, for a solution which has better 
	 * numerical stability. */
	return (1-t)*(1-t)*(1-t) * (p0_coord) + 3*t*(1-t)*(1-t)*(p1_coord) + 3*t*t*(1-t)*(p2_coord) + t*t*t*(p3_coord);	
}
