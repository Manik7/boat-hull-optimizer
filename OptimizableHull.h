#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <cassert>
#include "Optimizable.h"
#include "Hull.h"
#include "Bbox.h"

struct OptimizableHull : public Hull, public Optimizable {

	OptimizableHull();
	
	int get_parameter(int index) const;
	void set_parameter(int index, int value);
	bool satisfies_constraints() const;
	double fitness();

};

#endif // OPTIMIZABLE_HULL_H
