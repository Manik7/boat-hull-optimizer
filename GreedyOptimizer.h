#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

#include <random>

class GreedyOptimizer {

	Optimizable model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> distr;

public:
	GreedyOptimizer(Optimizable model);

	void do_step();

};

#endif //GREEDY_OPTIMIZER_H
