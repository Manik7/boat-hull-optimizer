#ifndef HULL_PARAMETERS_H
#define HULL_PARAMETERS_H

template<int NUMBER_OF_GENES>
struct HullParameters
{
// 	HullParameters() : MIN_VOLUME(0.75), MAX_TWIST_RATE_DEG(20.0) {}
	
	static constexpr double MIN_VOLUME = 0.75;
	static constexpr double MAX_TWIST_RATE_DEG = 20.0;
	static const int HALF_LWL = 3500;
	static const int HALF_BWL = 290;
	static const int MAX_DRAFT = 450;
	static const int STATION_SPACING = (3*HALF_LWL) / NUMBER_OF_GENES;
	static const int NUMBER_OF_STATIONS = (3*3500) / NUMBER_OF_GENES;
};
/*
HullParameters<NUMBER_OF_GENES>::MIN_VOLUME(0.75);
HullParameters<NUMBER_OF_GENES>::MAX_TWIST_RATE_DEG(20.0);*/

#endif // HULL_PARAMETERS_H