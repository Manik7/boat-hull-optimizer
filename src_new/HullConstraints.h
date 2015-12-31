#ifndef HULL_CONSTRAINTS_H
#define HULL_CONSTRAINTS_H

struct HullConstraints {
	double minVolume = 0.75;
	double maxTwistRateDeg = 20.0;
};

#endif // HULL_CONSTRAINTS_H