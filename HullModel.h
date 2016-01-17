#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "OptimizableModel.h"
#include "SharedStationProperties.h"
#include "WaterlineCurve.h"
#include "StationParameters.h"
#include "HullParameters.h"

//TODO: change this template parameter to be the number of stations

// template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO = 0, int DOMAIN_HI = 100>
class HullModel : public OptimizableModel//<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI> 
{
	using Station_properties = SharedStationProperties<int>;
	using Model = OptimizableModel;//<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>;
	
public: //attributes
	enum {CHINE_X = 0, CHINE_Y = 1, KEEL_Y = 2};
	
	//TODO should be static const, or maybe it's easier if you make them static constexpr
	StationParameters station_parameters[numberOfGenes/3];
	static constexpr HullParameters<numberOfGenes> hull_parameters = HullParameters<numberOfGenes>(); //TODO: You could make the hull model inherit from HullParameters, so you can access the values directly, which might clean up the code a little
	mutable Station_properties station_properties[numberOfGenes/3];
	bool station_properties_updated[numberOfGenes/3] = {false};
	
private: //attributes
	double volume = 0.0;
	double wetted_area = 0.0;
	double moment_to_trim_1_deg = 0.0;
	
public: //methods
	HullModel(); // Uses the default parameter values hard-coded into all the parameter-structs
	HullModel(std::pair< int, NumType > genome[]);
	void output() /*const*/; //TODO: do file output here as well, and not just console output
	void export_hull_coordinates(std::string filename) const;
	double compute_fitness(); //compute and return the fitness value //TODO: merge this with the fitness() function of the superclass???
	
	inline void set_parameter(int gene_index, int domain_value) {
		station_properties_updated[gene_index/3] = false;
		Optimizable_model::set_parameter(gene_index, domain_value);
	}
	
protected: //methods	
	/*TODO: as a performance improvement, you might be able to do with with a template parameter 
	 * to encourage to compiler to precompile one version of the function for each parameter. */
	inline NumType get_range_min(int gene_index) const { // Get the min value for the range (i.e. real values) for the parameters
		return 0;
	}
	inline NumType get_range_max(int gene_index) const {
		switch (gene_index % 3) {
			case CHINE_X:
				return station_parameters[gene_index/3].half_beam;
			case CHINE_Y:
				return hull_parameters.maxDraft;
			case KEEL_Y:
				return hull_parameters.maxDraft;
		}
	}

private: //methods	
	inline Station_properties calculate_station_properties(int station_index) {
		station_properties_updated[station_index] = true;
		return Station_properties(station_parameters[station_index].half_beam, 
					Model::genome[3*station_index+CHINE_X].second, 
					Model::genome[3*station_index+CHINE_Y].second, 
					Model::genome[3*station_index+KEEL_Y].second);
	}
	
	inline bool twist_rate_ok(Station_properties& first, Station_properties& second) const {
		return second.flare_rad < first.flare_rad + hull_parameters.maxTwistRateRad 
			&& second.deadrise_rad < first.deadrise_rad + hull_parameters.maxTwistRateRad; 
			
		/* NOTE: You do not check for an increasing panel twist here, meaning that 
		* decreasing flare & deadrise angles are possible. Furthermore, if the angles 
		* are decreasing, then that decrease is not limited to maxTwistRateDeg so 
		* it is hoped that the hulls will simply optimize such cases away.
		*/
	}
	
	inline double deg(double rad) const {
		return rad/3.14159265*180.0;
	}
};

#endif //HULL_MODEL_H