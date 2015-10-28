#ifndef BBOX_H
#define BBOX_H

#include "Point_3.h"

struct Bbox {
	Point_3 min, max;

	Bbox(Point_3 min = Point_3(), Point_3 max = Point_3()) : min(min), max(max) {}
};

#endif //BBOX_H
