#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

class GreedyOptimizer {

	Optimizable model;

public:
	GreedyOptimizer(Optimizable model) : model(model);

	void do_step();

};

#endif //GREEDY_OPTIMIZER_H
