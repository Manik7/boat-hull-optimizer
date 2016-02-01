#ifndef QSTATIONVIEW_H
#define QSTATIONVIEW_H

#include <QPointF>
#include "QT_Point_3.h"

struct StationView_qt {

    QPointF origin, beam, chine, keel;

    StationView_qt() : origin(QPointF(0,0)), beam(QPointF(300,0)), chine(QPointF(300,200)), keel(QPointF(0,450)) {}



};

#endif // QSTATIONVIEW_H
