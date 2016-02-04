#ifndef HULLDATAREADER_H
#define HULLDATAREADER_H

#include <string>
#include <fstream>
#include "Hull_qt.h"
#include "Point_3_qt.h"

struct HullDataReader {
    enum {keel = 0, chine = 1, beam = 2, origin = 3};

    Point_3_qt hull_data[5][4];

    HullDataReader();
    void read(std::string ifile);
    void write_hull_qt(Hull_qt& hull);
};

#endif // HULLDATAREADER_H
