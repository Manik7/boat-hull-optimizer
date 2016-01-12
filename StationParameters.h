#ifndef STATION_PARAMETERS_H
#define STATION_PARAMETERS_H

struct StationParameters {

	double flare_min_rad, flare_max_rad, deadrise_min_rad, deadrise_max_rad;
	int z_coord, half_beam;

	StationParameters(double fmin_deg, double fmax_deg, double dmin_deg, double dmax_deg) 
		: flare_min_rad(rad(fmin_deg)), flare_max_rad(rad(fmax_deg)), deadrise_min_rad(rad(dmin_deg)), deadrise_max_rad(rad(dmax_deg)) {}

	StationParameters() : StationParameters(0, 777) {}

	StationParameters(int z_coord, int half_beam) : flare_min_rad(rad(0.0)), flare_max_rad(rad(90.0)), deadrise_min_rad(rad(0.0)), deadrise_max_rad(rad(90.0)), z_coord(z_coord), half_beam(half_beam) {}
	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
};

#endif // STATION_PARAMETERS_H
