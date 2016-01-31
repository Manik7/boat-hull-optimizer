#include "HullModel.h"

HullModel::HullModel() 
	: station_parameters()
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
		Model::set_parameter(geneNo, valueDistribution(engine));
	}
}

void HullModel::output() /*const*/ {
	stream_output(std::cout);
}

void HullModel::stream_output(std::ostream& stream) /*const*/ {
	stream << "station\t" << "beam.x\t"<< "chine.x\t" << "chine.y\t" << "keel.y\t" << "area\t" << "per.\t" <<"flare\t" << "deadrise" << std::endl;
	
	for (int stat_no=0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
		station_properties[stat_no] = this->calculate_station_properties(stat_no); //force recalculation of the station properties just to be sure they are accurate
		
		stream << station_parameters[stat_no].z_coord << '\t' 
			<< station_parameters[stat_no].half_beam << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_X].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + CHINE_Y].second) << '\t' 
			<< std::to_string(Model::genome[3*stat_no + KEEL_Y].second) << '\t' 		
			<< station_properties[stat_no].area << '\t' << station_properties[stat_no].perimeter << '\t'
			<< deg(station_properties[stat_no].flare_rad) << '\t' << deg(station_properties[stat_no].deadrise_rad) << std::endl;
	}
	
	stream << "Properties of complete hull" << std::endl;
	stream << "Fitness = " << std::to_string(this->fitness()) << std::endl; //NOTE: fitness must be calcualted before outputting volume etc
	stream << "Volume =\t" << volume*4/(1000*1000*1000) << " m³" << std::endl << "WSA =\t" << wetted_area*4*pow(10,-6) << " m²\n" << std::endl;
}
	
void HullModel::export_hull(std::string filename) /*const*/ {
    export_hull_coordinates(filename);
    export_gnuplot_script(filename);
    export_hull_data(filename);
}

//TODO: change this so that the filename is seperate from the file ending
void HullModel::export_hull_coordinates(std::string filename) const {
	std::ofstream datfile;
	datfile.open("../../data/" + filename + ".dat");
	
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
	} else std::cout << "Error opening file: " << filename << ".dat" << std::endl;
}

void HullModel::export_gnuplot_script(std::string filename) const {
	std::ofstream gpScript;
	gpScript.open("../../data/" + filename + ".gp");
	
	if (gpScript.is_open()) {
		gpScript << "set terminal svg\n";
		gpScript << "set xrange [0:500]\n";
		gpScript << "set yrange [-500:0]\n";
		gpScript << "datafile = \"" << filename << ".dat\"\n";
		gpScript << "set output \"" << filename << ".svg\"\n";
		gpScript << "plot ";
	
		for (int s = 0; s < hull_parameters.numberOfStations; ++s) {
			gpScript << "datafile i " << s << " using 1:($2*-1) with lp lw 2 title columnheader(1)";
			if (s < hull_parameters.numberOfStations-1) gpScript << ", ";
			else gpScript << "\n";
		}
	    
		gpScript.close();
	} else std::cout << "Error opening file: " << filename << ".gp" << std::endl;
}

void HullModel::export_hull_data(std::string filename) /*const*/ {
	std::ofstream hulldata;
	hulldata.open("../../data/" + filename + ".txt");
	if (hulldata.is_open()) {
		stream_output(hulldata);
		hulldata.close();
	} else std::cout << "Error opening file: " << filename << ".txt" << std::endl;
}

/*TODO: Modifiy this function in such a way that the fitness is computed from several terms, 
 * each normalized to [0,1]. The additive constraint terms t_1...t_n are a measure of how close
 * the hull is to meeting that constraint. The multiplicative constraint terms c_1...c_n are 
 * binary, 1 when the constraint is met, else 0. The normalized Fitness terms f_WSA and f_pitch 
 * are subject to these multipliers. The overall formula is of the form:
 * 
 * F = t_1 + t_2 + t_3 + c_1*c_2*c_3*(f_WSA + f_pitch)
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
	
	//TODO twist
	double deadrise_c = 1., flare_c = 1., twist_c = 1.; //constraint multipliers
	double deadrise_t = 0, flare_t = 0, twist_t = 0; //additive terms
	
	for (int stat_no = 0; stat_no<hull_parameters.numberOfStations; ++stat_no) {
		
		if(!station_properties_updated[stat_no]) {
			station_properties[stat_no] = Station_properties(calculate_station_properties(stat_no));
			station_properties_updated[stat_no] = true;
		}
		
// 		if (!deadrise_constraint(stat_no)) {
// 			return 0.0; //deadrise NOT ok
// 		} else if (!flare_constraint(stat_no)) {
// 			return 0.0; // flare NOT ok
// 		} 
// 
// // 		else if (!twist_constraint(stat_no)) {
// // 			return 0.0;
// // 		}
		
		deadrise_c *= deadrise_constraint(stat_no);
		flare_c *= flare_constraint(stat_no);
// 		twist_c *= twist_constraint(stat_no);
		
		deadrise_t += station_deadrise_term(stat_no);
		flare_t += station_flare_term(stat_no);
// 		twist_t += station_twist_term(stat_no);
		
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
	
// 	if (volume > hull_parameters.minVolume) {
// 		return fitness_term();
// 	} else {
// 		return 0.0;
// 	}
	
	assert(0 <= volume_term() && volume_term() <= 1);
	assert(0 <= deadrise_t && deadrise_t <= 1);
	assert(0 <= flare_t && flare_t <= 1);
// 	assert(0 <= twist_t && twist_t <= 1);
	
	assert(flare_c == 0 || flare_c == 1);
	assert(deadrise_c == 0 || deadrise_c == 1);
// 	assert(twist_c == 0 || twist_c == 1);
	
	double volume_t(volume_term());
	double volume_c(volume_constraint());
	
	double result = volume_t + deadrise_t + flare_t + volume_c*deadrise_c*flare_c*fitness_term();
	if(result > 3.) {
		double x = 0.;	
	}
	assert(0 <= result && result <= 10);
	return result;
	
}
