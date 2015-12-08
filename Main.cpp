#include "GreedyOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	hull.generate_stations();	
	GreedyOptimizer greed(hull);
	greed.run();
	
	hull.compute_properties();
}
