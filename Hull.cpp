#include "Hull.h"

Hull::Hull(int half_lwl, int half_bwl, int number_of_stations) : half_lwl(half_lwl), half_bwl(half_bwl), 
number_of_stations(number_of_stations), station_spacing((half_lwl-100)/(number_of_stations-1)), wl_curve(WaterlineCurve(half_lwl, half_bwl)) {
	
	// ^ No stations generated for the last 100mm of the hull
	
	assert(number_of_stations >= 2);
	assert(half_lwl > 0 && half_bwl > 0);
	
	Station current_station;
	int station_bbox_width;
	int station_z_coord;
	
	Bbox bbox;
	StationParameters con;
	
	for (int i = 0; i<number_of_stations; i++) {

		station_z_coord = i*station_spacing;
		Point_3 wl_curve_point(wl_curve.find_point_with_z_coord(station_z_coord));

		bbox = Bbox(Point_3(0,0,i*station_spacing), Point_3(wl_curve_point.x, 450, station_z_coord));
		
		if (i==0) {
			con = StationParameters(0.0, 90.0, 0.0, 30.0);
		} else {
			con = StationParameters(0.0, 90.0, 0.0, 30.0);
		}
		
		stations.push_back(Station(bbox,con));
	}
}

void Hull::compute_properties() {
	
	volume = 0;
	wetted_surface_area = 0;
	pitching_moment = 0;
	
	for (auto it = stations.begin(); it != stations.end(); ++it) {
		volume += it->area() * station_spacing;
		wetted_surface_area += it->perimeter() * station_spacing;
		//TODO: pitching moment
	}
	
	// Subtract 1/2 the station spacing's worth of volume and area for the first and last station respectively
	volume -= station_spacing / 2 * (stations.begin()->area() + stations.end()->area());
	wetted_surface_area -= station_spacing / 2 * (stations.begin()->perimeter() + stations.end()->perimeter());	
	
	//volume = volume*4*pow(10,-9);
	//wetted_surface_area = wetted_surface_area*4*pow(10,-6);
}

void Hull::print_hull() const {
	
	Station::line_print_labels();
	for (auto station : stations) {
		station.line_print();
	}

	std::cout << "Properties of complete hull" << std::endl << "Volume =\t" << volume*4*pow(10,-9) << " m³" << std::endl << "WSA =\t" << wetted_surface_area*4*pow(10,-6) << " m²\n" << std::endl;
}

void Hull::export_hull_coordinates(std::string filename) const {
	std::ofstream datfile;
	datfile.open("data/" + filename);
	
	if (datfile.is_open()) {
	
		datfile << "# Station x y z coordinates\n";
		
		for (auto station : stations) {
			datfile << "\n\nstation-" << station.origin().z << '\n';
			datfile << station.keel().x << ' ' << station.keel().y << ' ' << station.keel().z << '\n';
			datfile << station.chine().x << ' ' << station.chine().y << ' ' << station.chine().z << '\n';
			datfile << station.beam().x << ' ' << station.beam().y << ' ' << station.beam().z << '\n';
			datfile << station.origin().x << ' ' << station.origin().y << ' ' << station.origin().z << '\n';
		}
		
		datfile.close();
	} else std::cout << "Error opening file!\n";
}