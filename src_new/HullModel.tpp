// #include "HullModel.h"

template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO, int DOMAIN_HI>
HullModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>::HullModel() 
	: station_parameters(), hull_parameters(HullParameters<NUMBER_OF_GENES>()), station_calculator(StationCalc(hull_parameters))
{
	WaterlineCurve wl_curve(hull_parameters.HALF_LWL, hull_parameters.HALF_BWL);
	
	for (int geneNo = 0; geneNo < NUMBER_OF_GENES; ++geneNo) {
		Model::genome[geneNo] = std::pair<int, T>(0, 0);
		Model::set_parameter(geneNo, DOMAIN_HI);
	}
	
	int statZCoord;
	for (int statNo = 0; statNo < hull_parameters.NUMBER_OF_STATIONS; ++statNo) {
		//compute the beams of the stations and initialize the station_parameters with those
		statZCoord = statNo*hull_parameters.STATION_SPACING;
		station_parameters[statNo] = StationParameters(statZCoord, wl_curve.x_coordinate(wl_curve.find_t_for_z_coord(statZCoord)));
	}
}

template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO, int DOMAIN_HI>
void HullModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>::output()
{
	StationProperties properties;
	
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "sq_per.\t" <<"flare\t" << "deadrise" << std::endl;
	
	for (int stat_no=0; stat_no<hull_parameters.NUMBER_OF_STATIONS; ++stat_no) {
		
		properties = calculate_station_properties(stat_no);
		
		std::cout << station_parameters[stat_no].z_coord << '\t' 
			<< station_parameters[stat_no].half_beam << '\t' 
/*			<< Model::genome[3*stat_no + CHINE_X] << '\t' 
			<< Model::genome[3*stat_no + CHINE_Y] << '\t' 
			<< Model::genome[3*stat_no + KEEL_Y] << '\t' */		
			<< '\t' << properties.area << '\t' << properties.sq_perimeter << '\t'
			<< properties.flare_deg << '\t' << properties.deadrise_deg << std::endl;
	}
}

template <typename T, int NUMBER_OF_GENES, int DOMAIN_LO, int DOMAIN_HI>
void HullModel<T, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>::compute_fitness()
{
	bool constraints_ok = true;
	StationProperties properties[hull_parameters.NUMBER_OF_STATIONS];
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
		
	if(constraints_ok) {
		// Subtract 1/2 the station spacing's worth of volume and area for the first and last station respectively
		volume -= hull_parameters.STATION_SPACING * (properties[0].area + properties[hull_parameters.NUMBER_OF_STATIONS-1].area) / 2;
		sq_wetted_area -= hull_parameters.STATION_SPACING * (properties[0].sq_perimeter + properties[hull_parameters.NUMBER_OF_STATIONS-1].sq_perimeter) / 2;	
		//TODO: moment_to_trim_1_deg
		
		Model::fitness = sq_wetted_area; //TODO: moment_to_trim_1_deg
	} else {
		Model::fitness = 0.0;
	}
	
}