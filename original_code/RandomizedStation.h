#ifndef RANDOMIZED_STATION_H
#define RANDOMIZED_STATION_H

#include "Station.h"

class RandomizedStation : public Station {

public:
	RandomizedStation(Bbox& bbox, Constraints& con) : Station(bbox, con) {}
	RandomizedStation() : Station() {}

protected:
	void generate_chine_and_keel();

};

#endif // RANDOMIZED_STATION_H
