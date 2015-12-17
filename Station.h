#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <random>
#include <cassert>

#include "Bbox.h"
#include "Point_3.h"
#include "Constraints.h"
/*#include "OptimizableHullFactory.h"
#include "Hull.h"*/

class Station {
	/*friend class OptimizableHullFactory;
	friend class Hull;*/

	/* TODO: Refactor so that the bbox, origin, beam, keel, and chine 
	 * are all private members, with friendship in the right places 
	 * to allow access. You could make the bbox and constraints 'const' 
	 * to protect against accidental modification */
	
	Bbox bbox_ = Bbox();
	Constraints constraints_ = Constraints();
	static const int number_of_iterations_ = 1000;
	static const int xy_resolution_ = 10; //TODO: Get rid of this, or continue to use it for the hull generation?
	Point_3 origin_, beam_, keel_, chine_;

public:
	Station(Bbox& bbox, Constraints& constraints);
	Station() {}

private:
	void generate_chine_and_keel();
	int resolution(int input) const;
	//TODO: Performance could be improved by using the squared_edge_length to avoid taking roots
	double edge_length(Point_3 a, Point_3 b) const;
	int sq_edge_length(Point_3 a, Point_3 b) const;
	bool points_in_bbox() const;

public:
	Bbox bbox() const;
	Point_3 origin() const;
	Point_3 beam() const;
	Point_3 keel() const;
	Point_3 chine() const;
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
