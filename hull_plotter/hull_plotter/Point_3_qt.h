#ifndef POINT_3_QT_H
#define POINT_3_QT_H

struct Point_3_qt {
//    enum {x = 0, y = 1, z = 2};

//    double coordinates[3];

//    Point_3_qt(double x, double y, double z) {
//        coordinates[0] = (x);
//        coordinates[1] = (y);
//        coordinates[2] = (z);
//    }

    int x,y,z;

    Point_3_qt(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
};

#endif // POINT_3_QT_H
