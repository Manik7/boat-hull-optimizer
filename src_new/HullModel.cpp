#include "HullModel.h"

HullModel::HullModel() 
	: station_parameters(), hull_parameters(HullParameters<numberOfGenes>()), station_calculator(StationCalc(hull_parameters))
{
	WaterlineCurve wl_curve(hull_parameters.halfLwl, hull_parameters.halfBwl);
	
	for (int geneNo = 0; geneNo < numberOfGenes; ++geneNo) {
		Model::genome[geneNo] = std::pair<int, NumType>(0, 0);
		Model::set_parameter(geneNo, domainHi);
	}
	
	int statZCoord;
	for (int statNo = 0; statNo < hull_parameters.numberOfStations; ++statNo) {
		//compute the beams of the stations and initialize the station_parameters with those
		statZCoord = statNo*hull_parameters.stationSpacing;
		station_parameters[statNo] = StationParameters(statZCoord, wl_curve.x_coordinate(wl_curve.find_t_for_z_coord(statZCoord)));
	}
}

HullModel::HullModel(std::pair< int, OptimizableModel::NumType > genome[]): OptimizableModel(genome), station_calculator(StationCalc(hull_parameters)) {}

void HullModel::output() const {
	StationProperties properties;
	
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "sq_per.\t" <<"flare\t" << "deadrise" << std::endl;
	
	for (int stat_no=0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
		properties = this->calculate_station_properties(stat_no);
		
		std::cout << station_parameters[stat_no].z_coord << '\t' 
			<< station_parameters[stat_no].half_beam << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_X].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_Y].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + KEEL_Y].second) << '\t' 		
			<< '\t' << properties.area << '\t' << properties.sq_perimeter << '\t'
			<< properties.flare_deg << '\t' << properties.deadrise_deg << std::endl;
	}
}

double HullModel::compute_fitness() const
{
	bool constraints_ok = true;
	StationProperties properties[hull_parameters.numberOfStations];
	volume = 0.0;
	sq_wetted_area = 0.0;
	moment_to_trim_1_deg = 0.0;
	
	for (int stat_no = 0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
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
		volume += properties[stat_no].area * hull_parameters.stationSpacing;
		sq_wetted_area += properties[stat_no].sq_perimeter * hull_parameters.stationSpacing;
		//TODO: moment_to_trim_1_deg
	}
		
	if(constraints_ok) {
		// Subtract 1/2 the station spacing's worth of volume and area for the first and last station respectively
		volume -= hull_parameters.stationSpacing * (properties[0].area + properties[hull_parameters.numberOfStations-1].area) / 2;
		sq_wetted_area -= hull_parameters.stationSpacing * (properties[0].sq_perimeter + properties[hull_parameters.numberOfStations-1].sq_perimeter) / 2;	
		//TODO: moment_to_trim_1_deg
		
		return sq_wetted_area; //TODO: moment_to_trim_1_deg
	} else {
		return 0.0;
	}
	
}