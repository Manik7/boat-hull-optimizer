#include "GreedyOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	hull.generate_stations();
	hull.compute_properties();
	hull.print_hull();
	hull.export_hull_coordinates("seed.dat");
	
	if(hull.satisfies_constraints()) {
		std::cout << "random hull successfully generated!\n\n";
		GreedyOptimizer<OptimizableHull> greed(hull);
		greed.run(1000000);
		hull.export_hull_coordinates("result.dat");
	}
}
