#ifndef STATION_PROPERTIES_H
#define STATION_PROPERTIES_H

struct StationProperties {
	const double area;
	const double sq_perimeter;
	const double flare_deg;
	const double deadrise_deg;
	
	StationProperties(double area, double sq_perimeter, double flare_deg, double deadrise_deg) : 
		area(area), sq_perimeter(sq_perimeter), flare_deg(flare_deg), deadrise_deg(deadrise_deg) {}
};

#endif // STATION_PROPERTIES_H