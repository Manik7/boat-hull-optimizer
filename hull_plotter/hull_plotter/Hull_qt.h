#ifndef HULL_QT_H
#define HULL_QT_H

#include <vector>
#include <iostream>

#include "Station_qt.h"

struct Hull_qt {
	std::vector<Station_qt> stations;

	Hull_qt() {
		Station_qt s0000(Point_3_qt(0, 419, 0), Point_3_qt(223, 282, 0), Point_3_qt(299, 0, 0), Point_3_qt(0, 0, 0));
		Station_qt s1100(Point_3_qt(0, 384, 1100), Point_3_qt(215, 256, 1100), Point_3_qt(288, 0, 1100), Point_3_qt(0, 0, 1100));
		Station_qt s2200(Point_3_qt(0, 300, 2200), Point_3_qt(169, 212, 2200), Point_3_qt(248, 0, 2200), Point_3_qt(0, 0, 2200));
		Station_qt s3300(Point_3_qt(0, 181, 3300), Point_3_qt(97, 124, 3300), Point_3_qt(166, 0, 3300), Point_3_qt(0, 0, 3300));
		Station_qt s4400(Point_3_qt(0, 23, 4400), Point_3_qt(15, 10, 4400), Point_3_qt(18, 0, 4400), Point_3_qt(0, 0, 4400));

		stations.push_back(s0000);
		stations.push_back(s1100);
		stations.push_back(s2200);
		stations.push_back(s3300);
		stations.push_back(s4400);
	}

	Hull_qt(Station_qt s0000, Station_qt s1100, Station_qt s2200, Station_qt s3300, Station_qt s4400) {
		stations.push_back(s0000);
		stations.push_back(s1100);
		stations.push_back(s2200);
		stations.push_back(s3300);
		stations.push_back(s4400);
	}

	void print_hull() {

		for (int st = 0; st < 5; ++st) {
			std::cout << "station #" << st << "\n";
			std::cout << stations[st].keel.x << " " << stations[st].keel.y << " " << stations[st].keel.z << "\n";
			std::cout << stations[st].chine.x << " " << stations[st].chine.y << " " << stations[st].chine.z << "\n";
			std::cout << stations[st].beam.x << " " << stations[st].beam.y << " " << stations[st].beam.z << "\n";
			std::cout << stations[st].origin.x << " " << stations[st].origin.y << " " << stations[st].origin.z << "\n";
			std::cout << "\n\n";
		}
	}

};

#endif // HULL_QT_H
