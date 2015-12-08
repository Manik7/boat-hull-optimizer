#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <random>
#include <cassert>

#include "Bbox.h"
#include "Point_3.h"
#include "Constraints.h"

class Station {
	typedef Point_3 Point;

	//TODO: You could make the bbox and constraints 'const' to protect against accidental modification
	Bbox bbox = Bbox();
	Constraints c = Constraints();
	static const int number_of_iterations = 1000;
	static const int xy_resolution = 10; //TODO: Better to not make this static? That would allow a higher resolution at the bow and lower resolution at the midsection
	Point origin, beam, keel, chine;

public:
	Station(Bbox& bbox, Constraints& constraints);
	Station() {}

private:
	void generate_chine_and_keel();
	int resolution(int input) const;
	//TODO: Performance could be improved by using the squared_edge_length to avoid taking roots
	double edge_length(Point a, Point b) const;
	int sq_edge_length(Point a, Point b) const;

public:
	Bbox getBbox() const;
	double area() const;
	double perimeter() const;
	int sq_perimeter() const;
	double z_coord() const;
	double area_perimeter_ratio() const;
	double area_sq_perimeter_ratio() const;
	double flare_angle_deg() const;
	double deadrise_angle_deg() const;
	bool satisfies_constraints() const; //TODO: Check the chine and keel against bbox constraints as well
	static void line_print_labels();
	void line_print() const;

	int get_parameter(int index);
	void set_parameter(int index, int value);
};

#endif // STATION_H
