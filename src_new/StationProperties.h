#ifndef STATION_PROPERTIES_H
#define STATION_PROPERTIES_H

struct StationProperties {
	//TODO these could all be const since the properties object is recreated each time
	double area;
	double perimeter;
	double flare_deg;
	double deadrise_deg;
	
	StationProperties(double area, double perimeter, double flare_deg, double deadrise_deg) : 
		area(area), perimeter(perimeter), flare_deg(flare_deg), deadrise_deg(deadrise_deg) {}
		
	StationProperties() {}
};

#endif // STATION_PROPERTIES_H