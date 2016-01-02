#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include <vector>
#include <iostream>

#include "OptimizableModel.h"
#include "StationCalculator.h"
#include "../StationParameters.h"
#include "../Bbox.h"
#include "HullParameters.h"

template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO = 0, int DOMAIN_HI = 100, double MUTATION_RATE = 0.01>
class HullModel : public OptimizableModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI, MUTATION_RATE> {
	
public: //attributes
	enum {CHINE_X = 0, CHINE_Y = 1, KEEL_Y = 2};
	
	//TODO should be static const
	StationParameters station_parameters[NUMBER_OF_GENES/3]; 
	HullParameters<NUMBER_OF_GENES> hull_parameters = HullParameters<NUMBER_OF_GENES>(); //TODO: You could make the hull model inherit from HullParameters, so you can access the values directly, which might clean up the code a little
	StationCalculator<int, HullParameters<NUMBER_OF_GENES> > station_calculator = StationCalculator(hull_parameters);
	
private: //attributes
	double volume = 0.0;
	double sq_wetted_area = 0.0;
	double moment_to_trim_1_deg = 0.0;
	
public: //methods
	HullModel(); // Uses the default parameter values hard-coded into all the parameter-structs
	void output(); //TODO: do file output here as well, and not just console output
	
protected: //methods
	void compute_fitness(); //compute and set the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with a template parameter 
	 * to encourage to compiler to precompile one version of the function for each parameter. */
	inline T get_range_min(int gene_index); // Get the min value for the range (i.e. real values) for the parameters
	inline T get_range_max(int gene_index);

private: //methods
	StationProperties calculate_station_properties(int station_index);
	void flare_angle_deg();
	inline bool twist_rate_ok(StationProperties& first, StationProperties& second);
};

#endif //HULL_MODEL_H