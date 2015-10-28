#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

struct Constraints {

	double area_min, area_max, flare_min, flare_max, deadrise_min, deadrise_max;

	Constraints(double amin, double amax, double fmin, double fmax, double dmin, double dmax) : area_min(amin), area_max(amax), flare_min(fmin), flare_max(fmax), deadrise_min(dmin), deadrise_max(dmax) {}

	Constraints() : area_min(0.0), area_max(0.0), flare_min(0.0), flare_max(0.0), deadrise_min(0.0), deadrise_max(0.0) {}

};

#endif // CONSTRAINTS_H
