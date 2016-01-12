#ifndef HULL_PARAMETERS_H
#define HULL_PARAMETERS_H

template<int NUMBER_OF_GENES>
struct HullParameters
{	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
	
	static constexpr double minVolume = 0.75 * 1000*1000*1000/4; //convert cubic meters for the total-hull into cubic millimeters for the quarter-hull
	static constexpr double maxTwistRateRad = rad(20.0);
	static constexpr int halfLwl = 3500;
	static constexpr int halfBwl = 290;
	static constexpr int maxDraft = 450;
	static constexpr int numberOfStations = NUMBER_OF_GENES / 3;
	static constexpr int stationSpacing = (halfLwl-100) / (numberOfStations - 1);
	
	

};
/*
HullParameters<NUMBER_OF_GENES>::MIN_VOLUME(0.75);
HullParameters<NUMBER_OF_GENES>::MAX_TWIST_RATE_DEG(20.0);*/

#endif // HULL_PARAMETERS_H