#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

#include <random>

class GreedyOptimizer {

	Optimizable model;
	static const int minValue = 0; //of parameter
	static const int maxValue = 100;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::uniform_int_distribution<int> diceRollDistribution;

public:
	GreedyOptimizer(Optimizable model);

	void run();
	void do_step(int maxStepSize);

private:
	int choose_parameter() const;
	bool win_dice_roll() const;

};

#endif //GREEDY_OPTIMIZER_H
