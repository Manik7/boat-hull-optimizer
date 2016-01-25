#include "HullModel.h"

HullModel::HullModel() 
	: /*OptimizableModel(),*/ station_parameters()
{
	WaterlineCurve wl_curve(hull_parameters.halfLwl, hull_parameters.halfBwl);
	
	int statZCoord;
	for (int statNo = 0; statNo < hull_parameters.numberOfStations; ++statNo) {
		//compute the beams of the stations and initialize the station_parameters with those
		statZCoord = statNo*hull_parameters.stationSpacing;
		station_parameters[statNo] = StationParameters(statZCoord, wl_curve.x_coordinate(wl_curve.find_t_for_z_coord(statZCoord)));
	}
	
	for (int geneNo = 0; geneNo < numberOfGenes; ++geneNo) {
		Model::genome[geneNo] = std::pair<int, NumType>(0, 0);
		Model::set_parameter(geneNo, domainHi);
	}
}

// HullModel::HullModel(std::pair< int, OptimizableModel::NumType > genome[], std::mt19937 engine): OptimizableModel(genome, engine) {}

HullModel::HullModel(std::mt19937 engine): OptimizableModel(engine) {
	for (int i = 0; i<numberOfGenes; ++i) {
		set_parameter(i, valueDistribution(engine));
	}
}


void HullModel::output() /*const*/ {
	std::cout << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "per.\t" <<"flare\t" << "deadrise" << std::endl;
	
	for (int stat_no=0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
		station_properties[stat_no] = this->calculate_station_properties(stat_no); //force recalculation of the station properties just to be sure they are accurate
		
		std::cout << station_parameters[stat_no].z_coord << '\t' 
			<< station_parameters[stat_no].half_beam << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_X].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_Y].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + KEEL_Y].second) << '\t' 		
			<< station_properties[stat_no].area << '\t' << station_properties[stat_no].perimeter << '\t'
			<< deg(station_properties[stat_no].flare_rad) << '\t' << deg(station_properties[stat_no].deadrise_rad) << std::endl;
	}
	
	std::cout << "Properties of complete hull" << std::endl;
	std::cout << "Fitness = " << std::to_string(this->fitness()) << std::endl; //NOTE: fitness must be calcualted before outputting volume etc
	std::cout << "Volume =\t" << volume*4*pow(10,-9) << " m³" << std::endl << "WSA =\t" << wetted_area*4*pow(10,-6) << " m²\n" << std::endl;
}

void HullModel::export_hull_coordinates(std::string filename) const {
	std::ofstream datfile;
	datfile.open("../../data/" + filename);
	
	if (datfile.is_open()) {
	
		datfile << "# Station x y z coordinates\n";
		
		int z_coord;
		
		for (int stat_no=0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
			z_coord = station_parameters[stat_no].z_coord;
			
			datfile << "\n\nstation-" << z_coord << '\n';
			datfile << 0 << ' ' << Model::genome[3*stat_no + KEEL_Y].second << ' ' << z_coord << '\n';
			datfile << Model::genome[3*stat_no + CHINE_X].second << ' ' << Model::genome[3*stat_no + CHINE_Y].second << ' ' << z_coord << '\n';
			datfile << station_parameters[stat_no].half_beam << ' ' << 0 << ' ' << z_coord << '\n';
			datfile << 0 << ' ' << 0 << ' ' << z_coord << '\n';
		}
		
		datfile.close();
	} else std::cout << "Error opening file!\n";
}

/*TODO: Modifiy this function in such a way that the fitness is computed from several terms, 
 * each normalized to [0,1]. The additive constraint terms c_1...c_n are a measure of how close
 * the hull is to meeting that constraint. The multiplicative constraint terms m_1...m_n are 
 * binary, 1 when the constraint is met, else 0. The normalized Fitness terms f_WSA and f_pitch 
 * are subject to these multipliers. The overall formula is of the form:
 * 
 * F = c_1 + c_2 + c_3 + m_1*m_2*m_3*(f_WSA + f_pitch)
 * 
 * This process ensures the algorithm first tries to satisfy all the constraints, and only then
 * starts working with the parameters which are to be optimized (WSA & pitch). Leaving the valid 
 * region is possible, but the fitness of such an inidivdual would be siginificantly lower than 
 * that of an individual who meets all the constraints.
 */
double HullModel::compute_fitness()
{
	volume = 0.0;
	wetted_area = 0.0;
	moment_to_trim_1_deg = 0.0;
	
	for (int stat_no = 0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
		if(!station_properties_updated[stat_no]) {
			station_properties[stat_no] = Station_properties(calculate_station_properties(stat_no));
			station_properties_updated[stat_no] = true;
		}
		
		if (station_parameters[stat_no].deadrise_min_rad > station_properties[stat_no].deadrise_rad || station_properties[stat_no].deadrise_rad > station_parameters[stat_no].deadrise_max_rad) {
			//deadrise NOT ok
			return 0.0;
		} else if (station_parameters[stat_no].flare_min_rad > station_properties[stat_no].flare_rad || station_properties[stat_no].flare_rad > station_parameters[stat_no].flare_max_rad) {
			// flare NOT ok
			return 0.0;
		} 
		
// 		else if (i>0) { //TODO: uncommment this twist rate check
// 			if(!twist_rate_ok(properties[i-1],properties[i])) {
// 				//twist-rate NOT ok
// 				return 0.0;
// 			}
// 		}
		
		//volume, WSA, moment to trim calcs
		volume += station_properties[stat_no].area;
		wetted_area += station_properties[stat_no].perimeter; //The stationSpacing must be squared as well for this to be the sq_WSA
		//TODO: moment_to_trim_1_deg
	}
		
	// Subtract 1/2 the station spacing's worth of volume and area for the first and last station respectively
	volume -= (station_properties[0].area + station_properties[hull_parameters.numberOfStations-1].area) / 2;
	wetted_area -= (station_properties[0].perimeter + station_properties[hull_parameters.numberOfStations-1].perimeter) / 2;
	//TODO: moment_to_trim_1_deg

	volume *= hull_parameters.stationSpacing;
	wetted_area *= hull_parameters.stationSpacing;
	
	if (volume > hull_parameters.minVolume) {
		return pow(10,6) / wetted_area; //TODO: moment_to_trim_1_deg
	} else {
		return 0.0;
	}
	
}