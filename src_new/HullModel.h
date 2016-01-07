#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include <vector>
#include <iostream>

#include "OptimizableModel.h"
#include "StationCalculator.h"
#include "../StationParameters.h"
#include "../Bbox.h"
#include "HullParameters.h"

//TODO: change this template parameter to be the number of stations, and simply compute the number of genes from there. 
template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO = 0, int DOMAIN_HI = 100>
class HullModel : public OptimizableModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI> {
	
	using Model = OptimizableModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>;
	using StationCalc = StationCalculator<int, HullParameters<NUMBER_OF_GENES> >;
	
public: //attributes
	enum {CHINE_X = 0, CHINE_Y = 1, KEEL_Y = 2};
	
	//TODO should be static const, or maybe it's easier if you make them static constexpr
	StationParameters station_parameters[NUMBER_OF_GENES/3]; 
	HullParameters<NUMBER_OF_GENES> hull_parameters; //TODO: You could make the hull model inherit from HullParameters, so you can access the values directly, which might clean up the code a little
	StationCalculator<int, HullParameters<NUMBER_OF_GENES> > station_calculator;
	
private: //attributes
	double volume = 0.0;
	double sq_wetted_area = 0.0;
	double moment_to_trim_1_deg = 0.0;
	
public: //methods
	HullModel(); // Uses the default parameter values hard-coded into all the parameter-structs
	void output(); //TODO: do file output here as well, and not just console output
	void compute_fitness(); //compute and set the fitness value
	
protected: //methods	
	/*TODO: as a performance improvement, you might be able to do with with a template parameter 
	 * to encourage to compiler to precompile one version of the function for each parameter. */
	inline T get_range_min(int gene_index) { // Get the min value for the range (i.e. real values) for the parameters
		return 0;
	}
	inline T get_range_max(int gene_index) {
		switch (gene_index % 3) {
			case CHINE_X:
				return station_parameters[gene_index/3].half_beam;
			case CHINE_Y:
				return hull_parameters.MAX_DRAFT;
			case KEEL_Y:
				return hull_parameters.MAX_DRAFT;
		}
	}

private: //methods
	void flare_angle_deg();
	
	inline StationProperties calculate_station_properties(int station_index) {
		return station_calculator.calculate_station_properties(station_parameters[station_index].half_beam, Model::genome[3*station_index]+CHINE_X, Model::genome[3*station_index]+CHINE_Y, Model::genome[3*station_index]+KEEL_Y);
	}
	
	inline bool twist_rate_ok(StationProperties& first, StationProperties& second) {
		return second.flare_deg < first.flare_deg + hull_parameters.MAX_TWIST_RATE_DEG 
			&& second.deadrise_deg < first.deadrise_deg + hull_parameters.MAX_TWIST_RATE_DEG; 
			
		/* NOTE: You do not check for an increasing panel twist here, meaning that 
		* decreasing flare & deadrise angles are possible. Furthermore, if the angles 
		* are decreasing, then that decrease is not limited to MAX_TWIST_RATE_DEG so 
		* it is hoped that the hulls will simply optimize such cases away.
		*/
	}
};

#endif //HULL_MODEL_H