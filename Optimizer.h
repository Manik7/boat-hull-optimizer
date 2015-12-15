#ifndef OPTIMIZER_H
#define OPTIMIZER_H

struct Optimizer {
	virtual void do_step() = 0;
	virtual void run(int steps = 5) = 0;	
};

#endif //OPTIMIZER_H