#ifndef OPTIMIZABLE_H
#define OPTIMIZABLE_H

struct Optimizable {

	const int numberOfParameters;

	virtual int getParameter(int index) const;
	virtual void setParameter(int index, int value);
	virtual bool checkContraints() const;
	virtual double fitness() const;

	Optimizable(const int numberOfParameters) : numberOfParameters(numberOfParameters) {};

};

#endif // OPTIMIZABLE_H
