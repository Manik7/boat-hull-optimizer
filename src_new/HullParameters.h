#ifndef HULL_PARAMETERS_H
#define HULL_PARAMETERS_H

template<int NUMBER_OF_GENES>
struct HullParameters
{
// 	HullParameters() : MIN_VOLUME(0.75), MAX_TWIST_RATE_DEG(20.0) {}
	
	static constexpr double minVolume = 0.75;
	static constexpr double maxTwistRateDeg = 20.0;
	static constexpr int halfLwl = 3500;
	static constexpr int halfBwl = 290;
	static constexpr int maxDraft = 450;
	static constexpr int stationSpacing = (3*(halfLwl-100)) / NUMBER_OF_GENES;
	static constexpr int numberOfStations = NUMBER_OF_GENES / 3;
};
/*
HullParameters<NUMBER_OF_GENES>::MIN_VOLUME(0.75);
HullParameters<NUMBER_OF_GENES>::MAX_TWIST_RATE_DEG(20.0);*/

#endif // HULL_PARAMETERS_H