#ifndef STATION_QT_H
#define STATION_QT_H

#include "Point_3_qt.h"

struct Station_qt {
    Point_3_qt keel, chine, beam, origin;

    Station_qt() : keel(Point_3_qt(0,0,0)), chine(Point_3_qt(300,0,0)), beam(Point_3_qt(300,200,0)), origin(Point_3_qt(0,450,0)) {}
    Station_qt(Point_3_qt keel, Point_3_qt chine, Point_3_qt beam, Point_3_qt origin) : keel(keel), chine(chine), beam(beam), origin(origin) {}
};

#endif // STATION_QT_H
