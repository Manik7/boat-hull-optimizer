#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <tgmath.h>

template <typename Number>
class Point {
public:
	static const int xmin = 0;
	static const int xmax = 300; //mm
	static const int ymin = 0;
	static const int ymax = 450;
	
	double x,y;
private:

	std::string name;

	void random_x() {
		this->x = rand() % (xmax-xmin) + xmin;
	}

	void random_y() {
		this->y = rand() % (ymax-ymin) + ymin;
	}

public:
	Point(std::string name = "unnamed") : name(name) {
		random_x();
		random_y();
	}

	//TODO: What about setting the flare angle to be fixed?
	Point(Number x_coord, Number y_coord, std::string name = "unnamed") : x(x_coord), y(y_coord), name(name) {
		if (x==-1.0) random_x();
		if (y==-1.0) random_y();
	}

	void print() {
		std::cout << name << "\t\t" << x << "\t" << y << std::endl;
	}
};


template <typename Number>
class Hull {
	typedef Point<Number> Point_t;
	
	Point_t origin, beam, keel, chine;
	
	Number edge_length(Point_t a, Point_t b) const {
		return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));
	}

public:

	Hull() : origin(0.0,0.0,"origin"), beam(Point_t::xmax,0.0,"beam"), keel(0.0, -1.0, "keel"), chine("chine") {}	

	void print() {
		chine.print();
		keel.print();
		std::cout << "area \t\t" << area() << std::endl;
		std::cout << "perimeter \t" << perimeter() << std::endl;
		std::cout << "A/P-ratio \t" << area_perimeter_ratio() << std::endl;
		std::cout << "flare\t\t" << flare_angle_deg() << std::endl;
		std::cout << "deadrise\t" << deadrise_angle_deg() << std::endl;
		std::cout << std::endl;
	}

	static void line_print_labels() {
		std::cout << "chine.x" << "\t" << "chine.y" << "\t" << "keel.y" 
			<< "\t" << "area" << "\t" << "perimeter" << "\t" << "a/p-ratio" << "\t"
			<<"flare" << "\t" << "deadrise" << std::endl;
	}

	void line_print() {
		std::cout << chine.x << "\t" << chine.y << "\t" << keel.y 
			<< "\t" << area() << "\t" << perimeter() << "\t" << area_perimeter_ratio() << "\t"
			<<flare_angle_deg() << "\t" << deadrise_angle_deg() << std::endl;
	}

	Number area() const {
		/*Calculate the area of the inner rectangular section
		 * and then add the deadrise triangle and the flare 
		 * triangle areas to that */

		return chine.x * chine.y 
			+ (beam.x - chine.x) * chine.y * 0.5
			+ chine.x * (keel.y - chine.y) * 0.5;
	}

	Number perimeter() const {
		return edge_length(beam, chine) + edge_length(chine, keel);
	}

	Number area_perimeter_ratio() {
		return area()/perimeter();
	}

	Number flare_angle_deg() {
		return acos(chine.y/edge_length(beam,chine)) / 3.1415 * 180.0;
	}

	Number deadrise_angle_deg() {
		return acos(chine.x/edge_length(chine, keel)) / 3.1415 * 180.0;

	}
	
};

int main() {
	srand (time(NULL));
		
	typedef double Number_t;
	typedef Hull<Number_t> Hull_t;

	Hull_t hull;
	Hull_t best_hull;
	Number_t best_ap_ratio = 0.0;
	unsigned int iterations = 0;;
	unsigned const int number_of_iterations = 10000000;
	unsigned int number_of_solutions = 0;
	
	hull.line_print_labels();

	while(iterations<number_of_iterations) {
		hull = Hull_t();
		if (hull.area() > 65000.0 && hull.area() < 70000) {
			number_of_solutions++;
//			hull.print();
			if(hull.area_perimeter_ratio() > best_ap_ratio) {
				best_hull = hull;
				best_ap_ratio = hull.area_perimeter_ratio();
				hull.line_print();
			}
		}
		iterations++;
	}
	std::cout << number_of_solutions << " viable solutions found in " << number_of_iterations << " iterations." << std::endl;
	return 0;
}
