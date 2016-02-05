#ifndef HULLPLOTTER_H
#define HULLPLOTTER_H

#include <QMainWindow>
#include <QPainter>
#include <QTransform>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QString>

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>

#include "Station_qt.h"
#include "Hull_qt.h"
#include "hulldatareader.h"
#include "counter.h"

namespace Ui {
class HullPlotter;
}

class HullPlotter : public QMainWindow
{
    Q_OBJECT

public:
    explicit HullPlotter(QWidget *parent = 0);
    ~HullPlotter();

private:
    Ui::HullPlotter *ui;
    //constexpr int numberOfStations = 5;
    //QT_Station stations[numberOfStations];

    QGraphicsScene* bodyPlanScene;
    QGraphicsScene* breadthPlanScene;
    QGraphicsScene* sheerPlanScene;
    QGraphicsEllipseItem* ellipse;

    QPen sheerPen;
    QPen stationPen;
    QPen chinePen;
    QPen keelPen;
    QPen defaultPen;
    QPen pointPen;

    QPen sheerPen_thick;
    QPen stationPen_thick;
    QPen chinePen_thick;
    QPen keelPen_thick;
    QPen defaultPen_thick;
    QPen pointPen_thick;

    Counter generation_number;
    unsigned int generation_step_size = 100000;

    std::vector<QGraphicsLineItem*> bodyPlanLines;
    std::vector<QGraphicsLineItem*> breadthPlanLines;
    std::vector<QGraphicsLineItem*> sheerPlanLines;

    std::vector<QPointF> chine_points;
    std::vector<QPointF> origin_points;
    std::vector<QPointF> beam_points;
    std::vector<QPointF> keel_points;

protected:
  void paintEvent(QPaintEvent *);

  void updateText(std::string filename);

  //looking from the front
  void body_plan(Hull_qt &hull);

  //looking from above
  void breadth_plan(Hull_qt& hull);

  //looking from the side
  void sheer_plan(Hull_qt& hull);
};
#endif // HULLPLOTTER_H
