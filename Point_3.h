#ifndef POINT_3_H
#define POINT_3_H

#include <string>
#include <math.h>

struct Point_3 {
	int x,y,z;
	std::string name;
	
	Point_3(int x_in = 0, int y_in = 0, int z_in = 0, std::string name = "_") : x(x_in), y(y_in), z(z_in), name(name) {} 

	int sq_distance(Point_3& p) {
		return pow(x-p.x, 2) + pow(y-p.y, 2) + pow(z-p.z, 2);
	}
	
	double distance(Point_3& p) {
		return sqrt(double(sq_distance(p)));
	}

};

inline std::ostream& operator<< (std::ostream& os, const Point_3& p) {
	os << p.name << '\t' << p.x << '\t' << p.y << '\t' << p.z;
	return os;
}

#endif // POINT_3_H
