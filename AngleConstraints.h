#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

struct AngleConstraints {

	double flare_min, flare_max, deadrise_min, deadrise_max;

	AngleConstraints(double fmin, double fmax, double dmin, double dmax) : flare_min(fmin), flare_max(fmax), deadrise_min(dmin), deadrise_max(dmax) {}

	AngleConstraints() : flare_min(0.0), flare_max(90.0), deadrise_min(0.0), deadrise_max(90.0) {}

};

#endif // CONSTRAINTS_H
