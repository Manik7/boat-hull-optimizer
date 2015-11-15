
#include <iostream>

#include "Point_3.h"
#include "Bezier_quadratic.h"

int main() {
	typedef Point_3 Point;
	typedef Bezier_quadratic Bezier;

	Point a(0.0, 0.0, 0.0, "p0");
	Point b(1000.0, 0.0, 0.0, "p1");
	Point c(1000.0, 0.0, 1000.0, "p2");

	Bezier curve(a,b,c);

	Point wp_00 = curve.point_at_t(0.0);
	Point wp_05 = curve.point_at_t(0.5);
	Point wp_10 = curve.point_at_t(1.0);

	std::cout << wp_00 << '\n';
	std::cout << wp_05 << '\n';
	std::cout << wp_10 << '\n';

	std::cout << '\n' << "z = 250 at:\n" << curve.find_point_with_z_coord(250.0) << std::endl;

	std::cout << '\n' << "z = 750 at:\n" << curve.find_point_with_z_coord(750.0) << std::endl;
}
