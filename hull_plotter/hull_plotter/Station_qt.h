#ifndef STATION_QT_H
#define STATION_QT_H

#include "Point_3_qt.h"

struct Station_qt {
    Point_3_qt origin, beam, chine, keel;

    Station_qt() : origin(Point_3_qt(0,0,0)), beam(Point_3_qt(300,0,0)), chine(Point_3_qt(300,200,0)), keel(Point_3_qt(0,450,0)) {}
    Station_qt(Point_3_qt origin, Point_3_qt beam, Point_3_qt chine, Point_3_qt keel) : origin(origin), beam(beam), chine(chine), keel(keel) {}
};

#endif // STATION_QT_H
