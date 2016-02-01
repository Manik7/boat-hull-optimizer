#ifndef HULL_QT_H
#define HULL_QT_H

#include <vector>
#include <QPointF>

#include "Station_qt.h"

struct Hull_qt {
    std::vector<Station_qt> stations;

    Hull_qt() {
        Station_qt s0000(Point_3_qt(0, 0, 0),Point_3_qt(0, 419, 0),Point_3_qt(223, 282, 0),Point_3_qt(299, 0, 0));
        Station_qt s1100(Point_3_qt(0, 0, 1100),Point_3_qt(0, 384, 1100),Point_3_qt(215, 256, 1100),Point_3_qt(288, 0, 1100));
        Station_qt s2200(Point_3_qt(0, 0, 2200),Point_3_qt(0, 300, 2200),Point_3_qt(169, 212, 2200),Point_3_qt(248, 0, 2200));
        Station_qt s3300(Point_3_qt(0, 0, 3300),Point_3_qt(0, 181, 3300),Point_3_qt(97, 124, 3300),Point_3_qt(166, 0, 3300));
        Station_qt s4400(Point_3_qt(0, 0, 4400),Point_3_qt(0, 23, 4400),Point_3_qt(15, 10, 4400),Point_3_qt(18, 0, 4400));

        stations.push_back(s0000);
        stations.push_back(s1100);
        stations.push_back(s2200);
        stations.push_back(s3300);
        stations.push_back(s4400);
    }

//    std::vector<std::vector<QPointF> > get_front_view() {
//        std::vector<QPointF> v;
//        for( auto iter : stations) {
//            vector<QPointF> st;
//            st.push_back(QPointF(iter.origin.x, iter.origin.y));
//            st.push_back(QPointF(iter.beam.x, iter.beam.y));
//            st.push_back(QPointF(iter.chine.x, iter.chine.y));
//            st.push_back(QPointF(iter.keel.x, iter.keel.y));
//            v.push_back(st);
//        }
//        return v;
//    }
};

#endif // HULL_QT_H
