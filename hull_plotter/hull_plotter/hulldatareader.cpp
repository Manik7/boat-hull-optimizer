#include "hulldatareader.h"

HullDataReader::HullDataReader()
{
}

void HullDataReader::read(std::string ifile) {
    std::ifstream ifs;

    ifs.open(ifile, std::ifstream::in);
    if (ifs.is_open()) {

        for (int st=0; st<5; ++st) {
            for (int pt=0; ifs.good() && pt<4; ++pt) {
                ifs >> hull_data[st][pt].x >> hull_data[st][pt].y >> hull_data[st][pt].z;
//                std::cout << hull_data[st][pt].x << " " << hull_data[st][pt].y << " " << hull_data[st][pt].z << "\n";
            }
        }
        ifs.get();
        ifs.close();
    } else {
        //std::cerr << "Failed to open pointcloud file!" << std::endl; //TODO: bug here
        /*emit invalidFile();*/
    }
}

void HullDataReader::write_hull_qt(Hull_qt& hull) {
    for (int station_no = 0; station_no < 5; ++station_no) {

        Point_3_qt keel_p = hull_data[station_no][keel];
        Point_3_qt chine_p = hull_data[station_no][chine];
        Point_3_qt beam_p = hull_data[station_no][beam];
        Point_3_qt origin_p = hull_data[station_no][origin];

        hull.stations.push_back(Station_qt(keel_p, chine_p, beam_p, origin_p));
    }
}
