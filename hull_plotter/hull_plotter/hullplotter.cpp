#include "hullplotter.h"
#include "ui_hullplotter.h"

HullPlotter::HullPlotter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HullPlotter)
{
    ui->setupUi(this);
}

HullPlotter::~HullPlotter()
{
    delete ui;
}

void HullPlotter::paintEvent(QPaintEvent *)
{
    QPointF a(50.,50.);
    QPointF b(100.,100.);

//    qStationView station;
    Hull_qt hull;

   QPainter painter(this);
//   painter.drawText(QRect(20,20,200,200),"Hello World");

   body_plan(hull, painter);
   breadth_plan(hull, painter);
   sheer_plan(hull, painter);
   painter.end();
}

//view from the front
void HullPlotter::body_plan(Hull_qt& hull, QPainter& painter) {
    for (auto& iter : hull.stations) {
        QPointF origin(iter.origin.x, iter.origin.y);
        QPointF beam(iter.beam.x, iter.beam.y);
        QPointF chine(iter.chine.x, iter.chine.y);
        QPointF keel(iter.keel.x, iter.keel.y);

        QTransform transform;
        transform.translate(350, 30);
        transform.scale(0.5,0.5);
        painter.setTransform(transform);

        painter.setPen(QColor(0,255,0));
        painter.drawLine(origin, beam);
        painter.setPen(QColor(255,0,0));
        painter.drawLine(beam, chine);
        painter.drawLine(chine, keel);
        painter.setPen(QColor(0,0,0));
        painter.drawLine(keel, origin);

        transform.scale(-1.0, 1.0);
        painter.setTransform(transform);

        painter.setPen(QColor(0,255,0));
        painter.drawLine(origin, beam);
        painter.setPen(QColor(255,0,0));
        painter.drawLine(beam, chine);
        painter.drawLine(chine, keel);
        painter.setPen(QColor(0,0,0));
        painter.drawLine(keel, origin);

    }
}

//looking from above
void HullPlotter::breadth_plan(Hull_qt& hull, QPainter& painter) {

    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.x, hull.stations[i].chine.z));
    }
    std::vector<QPointF> origin_points;
    for (int i = 4; i>=0; --i) {
        origin_points.push_back(QPointF(hull.stations[i].origin.x, hull.stations[i].origin.z));
    }
    std::vector<QPointF> beam_points;
    for (int i = 0; i<5; ++i) {
        beam_points.push_back(QPointF(hull.stations[i].beam.x, hull.stations[i].beam.z));
    }

    QTransform transform;
    transform.translate(100, 400);
    transform.scale(0.2,0.2);
    transform.rotate(-90);
    painter.setTransform(transform);

    painter.setPen(QColor(0,0,255));
    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

    painter.setPen(QColor(0,0,0));
    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

    painter.setPen(QColor(0,255,0));
    for (auto first = ++(beam_points.begin()), second = beam_points.begin(); first!=beam_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

    transform.scale(-1,1);
    painter.setTransform(transform);

    painter.setPen(QColor(0,0,255));
    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

    painter.setPen(QColor(0,0,0));
    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

    painter.setPen(QColor(0,255,0));
    for (auto first = ++(beam_points.begin()), second = beam_points.begin(); first!=beam_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }

/*    for (auto first = ++(points.begin()), second = points.begin(); first!=points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }*/

    //TODO: Plot stations
}

//looking from the side
void HullPlotter::sheer_plan(Hull_qt& hull, QPainter& painter) {
    painter.setPen(QColor(0,0,0));
    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.y, hull.stations[i].chine.z));
    }
    std::vector<QPointF> origin_points;
    for (int i = 4; i>=0; --i) {
        origin_points.push_back(QPointF(hull.stations[i].origin.y, hull.stations[i].origin.z));
    }
    std::vector<QPointF> keel_points;
    for (int i = 0; i<5; ++i) {
        keel_points.push_back(QPointF(hull.stations[i].keel.y, hull.stations[i].keel.z));
    }

    QTransform transform;
    transform.translate(100, 550);
    transform.scale(0.2,-0.2);
    transform.rotate(-90);
    painter.setTransform(transform);

    painter.setPen(QColor(0,0,255));
    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }
    painter.setPen(QColor(0,255,0));
    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }
    painter.setPen(QColor(0,0,0));
    for (auto first = ++(keel_points.begin()), second = keel_points.begin(); first!=keel_points.end(); ++first, ++second) {
        painter.drawLine(*first, *second);
    }


    //TODO: Plot stations
}
