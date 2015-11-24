#ifndef OPTIMIZABLE_H
#define OPTIMIZABLE_H

struct Optimizable {

	virtual int getParameter(int number) const;
	virtual void setParameter(int number, int value);
	virtual double fitness() const;

};

#endif // OPTIMIZABLE_H
