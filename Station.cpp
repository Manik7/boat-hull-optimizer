#include "Station.h"

Station::Station(Bbox& bbox, AngleConstraints& constraints) : bbox_(bbox), constraints_(constraints), origin_(bbox.min.x, bbox.min.y, bbox.min.z, "origin"), beam_(bbox.max.x, 0, origin_.z, "beam") {
		generate_chine_and_keel();		
}

void Station::generate_chine_and_keel() {
	std::random_device rd; // obtain a random number from hardware
	
#ifdef DETERMINISTIC_RUN
	std::mt19937 eng(0); // seed the generator
#else
	std::mt19937 eng(rd()); // seed the generator
#endif
	
	std::uniform_int_distribution<int> distr;
	bool foundSolution = false;

	chine_ = Point_3(0, 0, origin_.z, "chine"); //set the x-coordinate (beam) later

	for (int i = 0; i<number_of_iterations_; i++) {

		distr = std::uniform_int_distribution<int>(bbox_.max.x/4, bbox_.max.x); // set to x range, from 0 to bbox_max.x, instead of 0 I'm using 25%-of-beam instead, as solutions with the chine that far in would be discarded anyway
		chine_.x = distr(eng);  // random x_chine

		distr = std::uniform_int_distribution<int>(bbox_.max.y/4, bbox_.max.y); //set to y range, also using the 25% heuristic here
		chine_.y = distr(eng); // random y_chine
		keel_ = Point_3(0, distr(eng), origin_.z, "keel"); // random y_keel
		
		update();
		
		if (satisfies_constraints()) {
			foundSolution = true;
			break;
		}

	}
	if (!foundSolution) {
		//If you failed to find a solution, use this (very small) default station instead:
		chine_.x = xy_resolution_;
		chine_.y = bbox_.max.y/2;
		keel_ = Point_3(0, bbox_.max.y, origin_.z, "keel");
	}
	
	update();
}

int Station::resolution(int input) const {
	return input/xy_resolution_;
}

//TODO: Performance could be improved by using the squared_edge_length to avoid taking roots
double Station::edge_length(Point_3 a, Point_3 b) const {
	return sqrt(pow(double(a.x - b.x), 2.0) + pow(double(a.y - b.y), 2.0));
}

int Station::sq_edge_length(Point_3 a, Point_3 b) const {
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

Bbox Station::bbox() const {
	return bbox_;
}

Point_3 Station::origin() const
{
	return origin_;
}

Point_3 Station::chine() const
{
	return chine_;
}

Point_3 Station::beam() const
{
	return beam_;
}

Point_3 Station::keel() const
{
	return keel_;
}

double Station::area() const {
	return area_;
}

double Station::perimeter() const {
	return perimeter_;
}

int Station::sq_perimeter() const {
	return sq_edge_length(beam_, chine_) + sq_edge_length(chine_, keel_);
}

double Station::z_coord() const {
	return origin_.z;
}

double Station::area_perimeter_ratio() const {
	return area()/perimeter();
}

double Station::area_sq_perimeter_ratio() const {
	return double(area())/double(sq_perimeter());
}

double Station::flare_angle_deg() const { 
	return flare_angle_deg_;
}

double Station::deadrise_angle_deg() const { 
	return deadrise_angle_deg_;
}

bool Station::points_in_bbox() const //TODO: Only needed for the initialization of the station, since the greedy algorithm maintains this condition on its own accord
{
	if(chine_.x <= bbox_.max.x && chine_.y <= bbox_.max.y && keel_.y <= bbox_.max.y) return true;
	else return false;
}

bool Station::satisfies_constraints() const { // TODO: points_in_bbox can be removed here, only needed for initialization of the hull
	if (flare_angle_deg() >= constraints_.flare_min && flare_angle_deg() < constraints_.flare_max && deadrise_angle_deg() >= constraints_.deadrise_min && deadrise_angle_deg() < constraints_.deadrise_max) { 
		return true;
	} else return false;
}

void Station::line_print_labels() {
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "per.\t" << "a/p\t" <<"flare\t" << "deadrise" << std::endl;
}

void Station::line_print() const {
	std::cout << origin_.z << '\t' << beam_.x << '\t' << chine_.x << '\t' << chine_.y << '\t' << keel_.y 
		<< '\t' << area() << '\t' << perimeter() << '\t' << area_perimeter_ratio() << '\t'
		<<flare_angle_deg() << '\t' << deadrise_angle_deg() << std::endl;
}

int Station::get_parameter(int index) { //TODO: an enum for the parameters would be nice
	assert(index >= 0 && index < 3);
	
	switch (index) {
		case 0:
			return chine_.x;
		case 1:
			return chine_.y;
		case 2:
			return keel_.y;
	}
}

void Station::set_parameter(int index, int realValue) { //TODO: an enum for the parameters would be nice
	assert(index >= 0 && index < 3);
	
	switch (index) {
		case 0:
			chine_.x = realValue;
			break;
		case 1:
			chine_.y = realValue;
			break;
		case 2:
			keel_.y = realValue;
			break;
	}
	update();
}

void Station::update() {
	area_ = double(chine_.x * chine_.y + ( (beam_.x - chine_.x) * chine_.y + chine_.x * (keel_.y - chine_.y) )/2);
	perimeter_ = edge_length(beam_, chine_) + edge_length(chine_, keel_);
	flare_angle_deg_ = asin(chine_.y/edge_length(beam_,chine_)) / 3.1415 * 180.0; //TODO: Assumes point lies within bounding box
	
	if(keel_.y >= chine_.y) {
		deadrise_angle_deg_ = acos(chine_.x/edge_length(chine_, keel_)) / 3.1415 * 180.0; //TODO: Assumes points lie within bounding box
	} else {
		deadrise_angle_deg_ = -acos(chine_.x/edge_length(chine_, keel_)) / 3.1415 * 180.0; //TODO: Assumes points lie within bounding box
	}	
}