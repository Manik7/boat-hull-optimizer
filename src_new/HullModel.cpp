#include "HullModel.h"

HullModel::HullModel() : station_parameters(StationParameters[NUMBER_OF_GENES/3]),
	hull_parameters(HullParameters<NUMBER_OF_GENES>()),
	station_calculator(StationCalculator(hull_parameters))
{
	WaterlineCurve wl_curve(hull_parameters.HALF_LWL, hull_parameters.HALF_BWL);
	
	for (int geneNo = 0; geneNo < NUMBER_OF_GENES; ++geneNo) {
		OptimizableModel::genome[geneNo] = std::pair<int, T>(0, 0);
		OptimizableModel::set_parameter(geneNo, DOMAIN_HI);
	}
	
	for (int statNo : hull_parameters.NUMBER_OF_STATIONS) {
		//compute the beams of the stations and initialize the station_parameters with those
		station_parameters[statNo] = StationParameters(wl_curve.x_coordinate(wl_curve.find_t_for_z_coord(statNo*hull_parameters.STATION_SPACING)));
	}
}

void HullModel::output()
{
	StationProperties properties;
	
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "sq_per.\t" <<"flare\t" << "deadrise" << std::endl;
	
	for (int stat_no=0; stat_no<hull_parameters.NUMBER_OF_STATIONS; ++stat_no) {
		
		properties = calculate_station_properties(stat_no);
		
		std::cout << station_parameters[stat_no].z_coord << '\t' 
			<< station_parameters[stat_no].half_beam.x << '\t' 
			<< OptimizableModel::genome[3*stat_no + CHINE_X] << '\t' 
			<< OptimizableModel::genome[3*stat_no + CHINE_Y] << '\t' 
			<< OptimizableModel::genome[3*stat_no + KEEL_Y] << '\t' 		
			<< '\t' << properties.area << '\t' << properties.sq_perimeter << '\t'
			<< properties.flare_deg << '\t' << properties.deadrise_deg << std::endl;
	}
}


void HullModel::compute_fitness()
{
	bool constraints_ok = true;
	StationProperties properties[PROPERTIES.NUMBER_OF_STATIONS];
	volume = 0.0;
	sq_wetted_area = 0.0;
	moment_to_trim_1_deg = 0.0;
	
	for (int stat_no = 0; stat_no<hull_parameters.NUMBER_OF_STATIONS; ++stat_no) {
		
		properties[stat_no] = StationProperties(calculate_station_properties(stat_no));
		
		if (station_parameters[stat_no].deadrise_min > properties[stat_no].deadrise_deg || properties[stat_no].deadrise_deg > station_parameters[stat_no].deadrise_max) {
			//deadrise NOT ok
			constraints_ok = false;
			break;
		} else if (station_parameters[stat_no].flare_min > properties[stat_no].flare_deg || properties[stat_no].flare_deg > station_parameters[stat_no].flare_max) {
			// flare NOT ok
			constraints_ok = false;
			break;
		} 
		
// 		else if (i>0) { //TODO: uncommment this twist rate check
// 			if(!twist_rate_ok(properties[i-1],properties[i])) {
// 				constraints_ok = false;
// 				break;
// 			}
// 		}
		
		//volume, WSA, moment to trim calcs
		volume += properties[stat_no].area * hull_parameters.STATION_SPACING;
		sq_wetted_area += properties[stat_no].sq_perimeter * hull_parameters.STATION_SPACING;
		//TODO: moment_to_trim_1_deg
	}
	
	// Subtract 1/2 the station spacing's worth of volume and area for the first and last station respectively
	volume -= hull_parameters.STATION_SPACING * (properties[0].area + properties[hull_parameters.NUMBER_OF_STATIONS-1].area) / 2;
	sq_wetted_area -= hull_parameters.STATION_SPACING * (properties[0].sq_perimeter + properties[hull_parameters.NUMBER_OF_STATIONS-1].sq_perimeter) / 2;	
	//TODO: moment_to_trim_1_deg
	
	if(constraints_ok) {
		OptimizableModel::fitness = sq_wetted_area; //TODO: moment_to_trim_1_deg
	} else {
		OptimizableModel::fitness = 0.0;
	}
	
}

inline T HullModel::get_range_min(int gene_index) {
	return 0;
}

inline T HullModel::get_range_max(int gene_index) {
	switch (gene_index % 3) {
		case CHINE_X:
			return station_parameters[gene_index/3].half_beam;
		case CHINE_Y:
			return hull_parameters.MAX_DRAFT;
		case KEEL_Y:
			return hull_parameters.MAX_DRAFT;
	}
}

inline StationProperties HullModel::calculate_station_properties(int station_index)
{
	return station_calculator.calculate_station_properties(station_parameters[station_index].half_beam, OptimizableModel::genome[3*station_index]+CHINE_X, OptimizableModel::genome[3*station_index]+CHINE_Y, OptimizableModel::genome[3*station_index]+KEEL_Y);
}

inline bool HullModel::twist_rate_ok(StationProperties& first, StationProperties& second) {
	return second.flare_deg < first.flare_deg + hull_parameters.MAX_TWIST_RATE_DEG 
		&& second.deadrise_deg < first.deadrise_deg + hull_parameters.MAX_TWIST_RATE_DEG; 
		
	/* NOTE: You do not check for an increasing panel twist here, meaning that 
	 * decreasing flare & deadrise angles are possible. Furthermore, if the angles 
	 * are decreasing, then that decrease is not limited to MAX_TWIST_RATE_DEG so 
	 * it is hoped that the hulls will simply optimize such cases away.
	 */
}