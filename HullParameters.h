#ifndef HULL_PARAMETERS_H
#define HULL_PARAMETERS_H

template<int NUMBER_OF_GENES>
struct HullParameters
{	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
	
	static constexpr double minVolume = 0.85 * 1000*1000*1000/4; //convert cubic meters for the total-hull into cubic millimeters for the quarter-hull
	static constexpr double maxTwistRateRad = rad(20.0);
	static constexpr double keel_rise_rate = 0.1; // rocker. meters upwards per meter in the longitudinal direction
	static constexpr int halfLwl = 4750;
	static constexpr int halfBwl = 300;
	static constexpr int maxDraft = 450;
	static constexpr int numberOfStations = NUMBER_OF_GENES / 3;
	static constexpr int lastStationOffset = (halfLwl-100);
	static constexpr int stationSpacing = lastStationOffset / (numberOfStations - 1);
};

#endif // HULL_PARAMETERS_H