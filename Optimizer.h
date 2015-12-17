#ifndef OPTIMIZER_H
#define OPTIMIZER_H

struct Optimizer {
	
	/* TODO: You could program or document the need to have the template 
	 * argument OptimizableType and a constructor with the signature 
	 * Optimizer(OptimizableType& model) */
	
	virtual void do_step() = 0;
	virtual void run(int steps = 5) = 0;	
};

#endif //OPTIMIZER_H