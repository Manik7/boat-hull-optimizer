#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <assert>
#include "Optimizable.h"
#include "Hull.h"

class OptimizableHull : public Hull {

	int get_parameter(int index) const;
	void set_parameter(int index, int value);
	bool satisfies_constraints() const;
	double fitness() const;

public: 
	OptimizableHull() : numberOfParameters(3*number_of_stations), minValue(0), maxValue(99) {};

};

#endif // OPTIMIZABLE_HULL_H
