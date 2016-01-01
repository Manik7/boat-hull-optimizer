#ifndef FLYWEIGHT_STATION_H
#define FLYWEIGHT_STATION_H

#include "../Bbox.h"
#include "../AngleConstraints.h"

//TODO: Delete the whole class?

class FlyweightStation {
	
public:
	
	enum ParameterIndex { CHINE_X = 0, CHINE_Y = 1, KEEL_Y = 2};
	
	const Bbox& bbox;
	const StationParameters& angles;
	
	int chine_x, chine_y, keel_y;
	double area;
	double sq_perimeter;
	double flare_angle_deg;
	double deadrise_angle_deg;
	
private:
	bool isUpdated = false;
};

#endif // FLYWEIGHT_STATION_H
