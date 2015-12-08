#include "GreedyOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	hull.generate_stations();

	std::cout << "random hull generated, constraints met = " << hull.satisfies_constraints() << '\n';
	
	GreedyOptimizer<OptimizableHull> greed(hull);
	greed.run();
	
	hull.compute_properties();
}
