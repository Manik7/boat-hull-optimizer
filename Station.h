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
	//TODO: You could make the const to protect against accidental modification
	Bbox bbox = Bbox();
	Constraints c = Constraints();
	static const int number_of_iterations = 1000;
	static const int xy_resolution = 10; //TODO: Better to not make this static
	Point origin, beam, keel, chine;
	double m_area = 0.0;
	int m_sq_perimeter = 0;
	double m_perimeter = 0.0;
	double m_area_perimeter_ratio = 0.0;
	double m_area_sq_perimeter_ratio = 0.0;
	double m_flare_angle_deg = 0.0;
	double m_deadrise_angle_deg = 0.0;

public:
	Station(Bbox& bbox, Constraints& constraints) : bbox(bbox), c(constraints), origin(bbox.min.x, bbox.min.y, bbox.min.z), beam(bbox.max.x, 0, origin.z, "beam") {
		generate_chine_and_keel();		
	}

	Station() {}
private:

	void generate_chine_and_keel() {
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator
		std::uniform_int_distribution<int> distr;
		bool foundSolution = false;

		chine = Point(0, 0, origin.z, "chine");

		for (int i = 0; i<number_of_iterations; i++) {

			distr = std::uniform_int_distribution<int>(resolution(bbox.max.x)/4, resolution(bbox.max.x)); // set to x range
			chine.x = distr(eng)*xy_resolution;  // random x_chine

			distr = std::uniform_int_distribution<int>(resolution(bbox.max.y)/4, resolution(bbox.max.y)); //set to y range
			chine.y = distr(eng)*xy_resolution; // random y_chine
			keel = Point(0, distr(eng)*xy_resolution, origin.z, "keel"); // random y_keel
			
			if (satisfies_constraints()) {
				foundSolution = true;
				break;
			}

		}
		if (!foundSolution) {
			std::cout << "Could not find a solution for section at z = " << origin.z << " after " << number_of_iterations << " iterations." << std::endl;
		}
	}

	bool satisfies_constraints() {
		if (area() > c.area_min && area() < c.area_max && flare_angle_deg() > c.flare_min && flare_angle_deg() < c.flare_max && deadrise_angle_deg() > c.deadrise_min && deadrise_angle_deg() < c.deadrise_max) { 
			return true;
		} else return false;
	}

	int resolution(int input) {
		return input/xy_resolution;
	}

	//TODO: Performance could be improved by using the squared_edge_length to avoid taking roots
	double edge_length(Point a, Point b) const {
		return sqrt(pow(double(a.x - b.x), 2.0) + pow(double(a.y - b.y), 2.0));
	}

	int sq_edge_length(Point a, Point b) const {
		return pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0);
	}

public:
	double area() {
		/*if (m_area == 0.0) {*/
			/*Calculate the area of the inner rectangular section
			 * and then add the flare triangle and the deadrise 
			 * triangle areas to that. */
			return m_area = double(chine.x * chine.y 
				+ ( (beam.x - chine.x) * chine.y
				+ chine.x * (keel.y - chine.y) )/2);
		/*} else return m_area;*/
	}

	double perimeter() {
		/*if (m_perimeter == 0) {*/
			return m_perimeter = edge_length(beam, chine) + edge_length(chine, keel);
		/*} else return m_perimeter;*/
	}

	int sq_perimeter() {
		/*if (m_sq_perimeter == 0) {*/
			return m_sq_perimeter = sq_edge_length(beam, chine) + sq_edge_length(chine, keel);
		/*} else return m_sq_perimeter;*/
	}
	

	double area_perimeter_ratio() {
		/*if (m_area_perimeter_ratio == 0.0) {*/
			return m_area_perimeter_ratio = area()/perimeter();
		/*} else return m_area_perimeter_ratio;*/
	}

	double area_sq_perimeter_ratio() {
		/*if (m_area_sq_perimeter_ratio == 0.0) {*/
			return m_area_sq_perimeter_ratio = double(area())/double(sq_perimeter());
		/*} else return m_area_sq_perimeter_ratio;*/
	}

	double flare_angle_deg() {
		/*if (m_flare_angle_deg == 0.0) {*/
			return m_flare_angle_deg = acos(double(chine.y)/edge_length(beam,chine)) / 3.1415 * 180.0;
		/*} else return m_flare_angle_deg;*/
	}

	double deadrise_angle_deg() {
		/*if (m_deadrise_angle_deg == 0.0) {*/
			return m_deadrise_angle_deg = acos(chine.x/edge_length(chine, keel)) / 3.1415 * 180.0;
		/*} else return m_deadrise_angle_deg;*/
	}

	static void line_print_labels() {
		std::cout << "chine.x" << "\t" << "chine.y" << "\t" << "keel.y" 
			<< "\t" << "area" << "\t" << "per." << "\t" << "a/p" << "\t"
			<<"flare" << "\t" << "deadrise" << std::endl;
	}

	void line_print() {
		std::cout << chine.x << "\t" << chine.y << "\t" << keel.y 
			<< "\t" << area() << "\t" << perimeter() << "\t" << area_perimeter_ratio() << "\t"
			<<flare_angle_deg() << "\t" << deadrise_angle_deg() << std::endl;
	}

};

#endif // STATION_H
