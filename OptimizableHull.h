#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <assert>
#include "Optimizable.h"

class OptimizableHull : public Hull {

	int get_parameter(int index) const;
	void set_parameter(int index, int value);
	bool satisfies_constraints() const;
	double fitness() const;

	const int numberOfParameters;

public: 
	OptimizableHull() : numberOfParameters(3*number_of_stations) {};

};

#endif // OPTIMIZABLE_HULL_H
