#include "HullModel.h"

bool HullModel::satisfies_constraints(StationProperties& properties) {
	
	
}

void HullModel::compute_fitness()
{
	bool constraints_ok = true;
	StationProperties properties;
	volume = 0.0;
	sq_wetted_area = 0.0;
	moment_to_trim_1_deg = 0.0;
	
	for (int i = 0; i<NUMBER_OF_PARAMETERS/3; ++i) {
		
		properties = StationProperties(HullModel.station_builder.constructStation(
			OptimizableModel::parameters[i*3], OptimizableModel::parameters[i*3+1], OptimizableModel::parameters[i*3+2]));
		
		if (angle_constraints[i].deadrise_min > properties.deadrise_deg || properties.deadrise_deg > angle_constraints[i].deadrise_max) {
			//deadrise NOT ok
			constraints_ok = false;
		} else if (angle_constraints[i].flare_min > properties.flare_deg || properties.flare_deg > angle_constraints[i].flare_max) {
			// flare NOT ok
			constraints_ok = false;
		} //TODO: implement twist rate here (with another else-if)
		
		//TODO: volume, sq_WSA, and pitching moment calculations and additions here
	}
	
	if(constraints_ok) {
		OptimizableModel::fitness = sq_wetted_area; //TODO
	} else {
		OptimizableModel::fitness = 0.0;
	}
	
}

T HullModel::get_range_max(int index)
{
// TODO
}

T HullModel::get_range_min(int index)
{
// TODO
}