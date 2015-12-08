#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <cassert>
#include "Optimizable.h"
#include "Hull.h"
#include "Bbox.h"

struct OptimizableHull : public Hull, public Optimizable {

	int get_parameter(int index) const;
	void set_parameter(int index, int value);
	bool satisfies_constraints() const;
	double fitness();

	OptimizableHull() : Optimizable(3*number_of_stations, 0, 99) {};

};

#endif // OPTIMIZABLE_HULL_H
