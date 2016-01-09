#ifndef STATION_PARAMETERS_H
#define STATION_PARAMETERS_H

struct StationParameters {

	double flare_min, flare_max, deadrise_min, deadrise_max;
	int z_coord, half_beam;

	StationParameters(double fmin, double fmax, double dmin, double dmax, int half_beam = 777) : flare_min(fmin), flare_max(fmax), deadrise_min(dmin), deadrise_max(dmax), half_beam(half_beam) {}

	StationParameters() : flare_min(0.0), flare_max(90.0), deadrise_min(0.0), deadrise_max(90.0), z_coord(0), half_beam(777) {}

	StationParameters(int z_coord, int half_beam) : flare_min(0.0), flare_max(90.0), deadrise_min(0.0), deadrise_max(90.0), z_coord(z_coord), half_beam(half_beam) {}
};

#endif // STATION_PARAMETERS_H
