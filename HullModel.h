#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>

#include "OptimizableModel.h"
#include "SharedStationProperties.h"
#include "WaterlineCurve.h"
#include "StationParameters.h"
#include "HullParameters.h"

//TODO: change this template parameter to be the number of stations

class HullModel : public OptimizableModel//<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI> 
{
	using Station_properties = SharedStationProperties<int>;
	using Model = OptimizableModel;
	
public: //attributes
	enum {CHINE_X = 0, CHINE_Y = 1, KEEL_Y = 2};
	
	StationParameters station_parameters[numberOfGenes/3];
	static constexpr HullParameters<numberOfGenes> hull_parameters = HullParameters<numberOfGenes>(); //TODO: You could make the hull model inherit from HullParameters, so you can access the values directly, which might clean up the code a little
	mutable Station_properties station_properties[numberOfGenes/3];
	bool station_properties_updated[numberOfGenes/3] = {false};
	int birth_generation = 0;
	
private: //attributes
	double volume = 0.;
	double wetted_area = 0.;
	double moment_to_trim_1_deg = 0.;
	double prismatic_coefficient = 0.;
	
public: //methods
	HullModel(); // Uses the default parameter values hard-coded into all the parameter-structs
	
	void output(); //TODO: do file output here as well, and not just console output
	void stream_output(std::ostream& stream);
	void export_hull(std::string filename);
	void export_hull_coordinates(std::string filename) const;
	void export_gnuplot_script(std::string filename) const;
	void export_point_cloud(std::string filename) const;
	void export_hull_data(std::string filename) /*const*/;
	double compute_fitness(); //compute and return the fitness value //TODO: merge this with the fitness() function of the superclass???
	
	inline void set_parameter(int gene_index, int domain_value) {
		station_properties_updated[gene_index/3] = false;
		Optimizable_model::set_parameter(gene_index, domain_value);
	}
	
	
protected: //methods	
	/*TODO: as a performance improvement, you might be able to do with with a template parameter 
	 * to encourage to compiler to precompile one version of the function for each parameter. */
	inline NumType get_range_min(int gene_index) const { // Get the min value for the range (i.e. real values) for the parameters
		return 10; //having this != 0 ensures that the chine point cannot lie exactly on the keel-point or beam-point
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
	
	static constexpr double deg(double rad) {
		return rad/3.14159265*180.0;
	}
	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
	
	inline int volume_constraint() {
		if (volume > hull_parameters.minVolume) {
			return 1;
		} else return 0;
	}
	
	inline int deadrise_constraint(int stat_no) {
		if (station_parameters[stat_no].deadrise_min_rad <= station_properties[stat_no].deadrise_rad && station_properties[stat_no].deadrise_rad <= station_parameters[stat_no].deadrise_max_rad) {
			return 1; //deadrise OK
		} else return 0;
	}
	
	inline int flare_constraint(int stat_no) {
		if (station_parameters[stat_no].flare_min_rad <= station_properties[stat_no].flare_rad && station_properties[stat_no].flare_rad <= station_parameters[stat_no].flare_max_rad) {
			return 1; //flare OK
		} else return 0;
	}
	
	inline int twist_constraint(int stat_no) {
		if (stat_no>0) { 
			if(!twist_rate_ok(station_properties[stat_no-1],station_properties[stat_no])) {
				return 0; //twist-rate NOT ok
			} else return 1;
		} else return 1;
	}
	
	
	/* check if all the beam and chine values of a station are smaller than those of its predecessor
	 * or alternatively, just chekc that the area is smaller than the predeccessor */
	inline int convergence_constraint(int stat_no) {
		if (stat_no>0) {
			if(station_properties[stat_no].area < station_properties[stat_no-1].area) {
				return 1;
			}
			else return 0; //this station is too large
		} else return 1; //this is the 0th station
	}
	
	inline double volume_term() {
		if (volume > hull_parameters.minVolume) {
			return 1.0;
		} else {
			return volume/hull_parameters.minVolume;
		}
	}
	
	inline double station_deadrise_term(int stat_no) {
		double result;
		
		double d_min = station_parameters[stat_no].deadrise_min_rad;
		double d_max = station_parameters[stat_no].deadrise_max_rad;
		double d = station_properties[stat_no].deadrise_rad;
		assert (rad(-90) <= d && d <= rad(90));
		
		if(d_min <= d && d <= d_max) {
			result = 1.;
		} else if (d < d_min) {
			result = (d + rad(90)) / (d_min + rad(90));
		} else if (d > d_max) {
			result = 1-(d-d_max)/(rad(90) - d_max);
		}
		
		assert(0. <= result && result <= 1.);
		return result / hull_parameters.numberOfStations;
	}
	
	// this function returns the flare term for one station
	inline double station_flare_term(int stat_no) {
		double result;
		double f_min = station_parameters[stat_no].flare_min_rad;
		double f_max = station_parameters[stat_no].flare_max_rad;
		double f = station_properties[stat_no].flare_rad;
		assert (0 <= f && f <= rad(90));
		
		if (f_min <= f && f <= f_max) {
			result = 1.;
		} else if (f < f_min) {
			result = f / f_min;
		} else if (f > f_max) {
			result = 1-(f - f_max) / (rad(90) - f_max);
		} else {
			assert(false); // flare invalid, i.e. not in [0°,90°]
			result = 0.;
		}
		assert(0. <= result && result <= 1.);
		return result / hull_parameters.numberOfStations;
	}
	
	inline double station_twist_term(int stat_no) {
		//TODO
		return 0.;
	}
	
	//TODO: Can this function be combined with the convergence_constraint for improved performance? 
	inline double station_convergence_term(int stat_no) {	
		double result;
		
		if(stat_no == 0) { //first station
			result = 1.;
		} else { 
			double prev_area = station_properties[stat_no-1].area;
			double curr_area = station_properties[stat_no].area;
			
			if (curr_area <= prev_area) {
				result = 1.;
			} else {
				result = prev_area/curr_area;
			}
		}
				
		assert(0. <= result && result <= 1.);
		return result / hull_parameters.numberOfStations;
	}
	
	inline double fitness_term() {
		//TODO: moment_to_trim_1_deg
// 		return 4*250*1000 / (wetted_area-1000*1000); 
// 		return (prismatic_coefficient-0.4)*2;
// 		return prismatic_coefficient;
		return 4*250*1000 / (wetted_area-1000*1000) + (prismatic_coefficient-0.4)*2;
		/* I highly doubt the overall surface area of the hull will drop below 5m^2 no matter 
		 * how much optimization is done. 1m^2 is subtracted from the WSA of the quarter-hull
		 * so that changes in the (remaining) WSA have a much larger impact on the fitness 
		 * function. Previously a 'huge' change in WSA from 6.0 to 5.5 would only have had a very
		 * small effect on the overall fitness, especially with all the constraint terms going 
		 * into the fitness as well. The result has been divided by 4, so that a hull with a WSA
		 * of 5 would have a fitness term of 1.
		 */
	}
	
	
};

#endif //HULL_MODEL_H
