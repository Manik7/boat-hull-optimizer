#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <random>

#include "Bbox.h"
#include "Point_3.h"
#include "Constraints.h"

class Station {

	typedef Point_3 Point;

private:
	//TODO: You could make the bbox and constraints const to protect against accidental modification
	Bbox bbox = Bbox();
	Constraints c = Constraints();
	static const int number_of_iterations = 1000;
	static const int xy_resolution = 10; //TODO: Better to not make this static
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
	double area() const;
	double perimeter() const;
	int sq_perimeter() const;
	double z_coord() const;
	double area_perimeter_ratio() const;
	double area_sq_perimeter_ratio() const;
	double flare_angle_deg() const;
	double deadrise_angle_deg() const;
	bool satisfies_constraints() const;
	static void line_print_labels();
	void line_print() const;

};

#endif // STATION_H
