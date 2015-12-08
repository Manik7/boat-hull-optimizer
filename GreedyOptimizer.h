#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

#include <random>
#include "Optimizable.h"

template<typename OptimizableType>
class GreedyOptimizer {

	OptimizableType model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::uniform_int_distribution<int> diceRollDistribution;

public:
	GreedyOptimizer(OptimizableType model);

	void run();
	void do_step();

private:
	int choose_parameter() const;
	bool win_dice_roll();

};

#endif //GREEDY_OPTIMIZER_H
