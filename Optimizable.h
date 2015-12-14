#ifndef OPTIMIZABLE_H
#define OPTIMIZABLE_H

struct Optimizable {

	const int numberOfParameters;
	const int minValue;
	const int maxValue;

	Optimizable(const int numberOfParameters, const int minValue, const int maxValue) : numberOfParameters(numberOfParameters), minValue(minValue), maxValue(maxValue) {};
	
	virtual int get_parameter(int index) const=0;
	virtual void set_parameter(int index, int value)=0;
	virtual void revert_last_change()=0;
	virtual bool satisfies_constraints() const =0;
	virtual double fitness()=0;
	virtual void output()=0;
};

#endif // OPTIMIZABLE_H
