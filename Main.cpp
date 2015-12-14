#include "GreedyOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	hull.generate_stations();
	hull.compute_properties();
	hull.print_hull();
	
	std::cout << "random hull generated, satisfies_constraints() = " << hull.satisfies_constraints() << "\n\n\n";
	
	GreedyOptimizer<OptimizableHull> greed(hull);
	greed.run(1000);
	hull.export_hull_coordinates();	
}
