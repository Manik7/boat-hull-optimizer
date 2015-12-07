#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <assert>
#include "Optimizable.h"

class OptimizableHull : public Hull {

	int getParameter(int index) const;
	void setParameter(int index, int value);
	bool checkContraints() const;
	double fitness() const;

	const int numberOfParameters;

public: 
	OptimizableHull() : numberOfParameters(3*number_of_stations) {};

};

#endif // OPTIMIZABLE_HULL_H
