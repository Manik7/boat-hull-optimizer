#include "Hull.h"

Hull::Hull(int half_lwl, int half_bwl, int number_of_stations) : half_lwl(half_lwl), half_bwl(half_bwl), number_of_stations(number_of_stations), wl_curve(WaterlineCurve(half_lwl, half_bwl)) {
	
}

void Hull::generate_stations() {

	double station_spacing = (half_lwl-100)/(number_of_stations-1); //No stations generated for the last 100mm of the hull
	int station_bbox_width;
	int station_z_coord;

	for (int i = 0; i<number_of_stations; i++) {

		station_z_coord = i*station_spacing;
		Point_3 wl_curve_point(wl_curve.find_point_with_z_coord(station_z_coord));

		Bbox bbox(	Point_3(0,0,i*station_spacing),
				Point_3(wl_curve_point.x, 450, station_z_coord));

		Constraints con;

		if (i==0) {
			con = Constraints(40000, 60000, 0.0, 25.0, 0.0, 70.0);
		} else {
			con = Constraints(0, 60000, 0.0, 25.0, 0.0, 90.0);
		}

		generate_optimized_station(bbox, con);
	}
}

void Hull::compute_properties() {
	
	assert(number_of_stations >= 2);

	//Reset Hull attributes to 0.0
	volume = 0.0;
	wetted_surface_area = 0.0;
	pitching_moment = 0.0;
	
	double station_spacing = 0.0;
	auto it = stations.begin();
	++it;
	auto it_prev = stations.begin();

	for ( ; it != stations.end(); ++it, ++it_prev) {
		station_spacing = it->z_coord() - it_prev->z_coord();

		volume += (it_prev->area() + it->area())/2.0 * station_spacing;
		wetted_surface_area += (it_prev->perimeter() + it->perimeter())/2.0 * station_spacing;
		//TODO: pitching moment
	}	
	
	volume = volume*4*pow(10,-9);
	wetted_surface_area = wetted_surface_area*4*pow(10,-6);
}

void Hull::print_hull() const {
	
	Station::line_print_labels();
	for (auto station : stations) {
		station.line_print();
	}

	std::cout << "Properties of complete hull" << std::endl << "Volume =\t" << volume << " m³" << std::endl << "WSA =\t" << wetted_surface_area << " m²\n" << std::endl;
}

void Hull::export_hull_coordinates(std::string filename) const {
	std::ofstream datfile;
	datfile.open(filename);
	
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


void Hull::generate_optimized_station(Bbox& bbox, Constraints& con) {

	Station station;
	Station best_station;
	double best_ap_ratio = 0.0;
	unsigned int iterations = 0;
	const unsigned int number_of_iterations = 100;

	while(iterations<number_of_iterations) {
		station = Station(bbox, con);

		if(station.area_perimeter_ratio() > best_ap_ratio) {
			best_ap_ratio = station.area_perimeter_ratio();
			best_station = station;		
		}
		iterations++;
	}
	stations.push_back(best_station);
}
