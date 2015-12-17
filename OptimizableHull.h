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
	void set_all_parameters(int value);
	void revert_last_change();
	bool satisfies_constraints() const;
	double fitness();
	void output();
	
private:
	bool parameterChanged = false;
	int lastParameterIndex = 0;
	int lastParameterRealValue = 0;

};

#endif // OPTIMIZABLE_HULL_H
