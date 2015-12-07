#ifndef OPTIMIZABLE_H
#define OPTIMIZABLE_H

struct Optimizable {

	virtual int getParameter(int index) const;
	virtual void setParameter(int index, int value);
	virtual bool checkContraints() const;
	virtual double fitness() const;
};

#endif // OPTIMIZABLE_H
