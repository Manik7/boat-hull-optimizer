#ifndef OPTIMIZABLE_HULL_H
#define OPTIMIZABLE_HULL_H

#include <assert>
#include "Optimizable.h"

class OptimizableHull : public Hull {

	int getParameter(int index) const;
	void setParameter(int index, int value);
	double fitness() const;

};

#endif // OPTIMIZABLE_HULL_H
