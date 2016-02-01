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

   for (auto& iter : hull.stations) {
       QPointF origin(iter.origin.x, iter.origin.y);
       QPointF beam(iter.beam.x, iter.beam.y);
       QPointF chine(iter.chine.x, iter.chine.y);
       QPointF keel(iter.keel.x, iter.keel.y);

       QTransform transform;
       transform.translate(350, 30);
       transform.scale(0.5,0.5);
       painter.setTransform(transform);

       painter.drawLine(origin, beam);
       painter.drawLine(beam, chine);
       painter.drawLine(chine, keel);
       painter.drawLine(keel, origin);

       transform.scale(-1.0, 1.0);
       painter.setTransform(transform);

       painter.drawLine(origin, beam);
       painter.drawLine(beam, chine);
       painter.drawLine(chine, keel);
       painter.drawLine(keel, origin);

   }



   painter.end();
}
