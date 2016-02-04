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

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>

#include "Station_qt.h"
#include "Hull_qt.h"
#include "hulldatareader.h"

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
    QPen defaultPen;
    QPen pointPen;

    std::vector<QGraphicsLineItem*> bodyPlanLines;

protected:
  void paintEvent(QPaintEvent *);

  //looking from the front
  void body_plan(Hull_qt &hull);

  //looking from above
  void breadth_plan(Hull_qt& hull, QPainter& painter);

  //looking from the side
  void sheer_plan(Hull_qt& hull, QPainter& painter);
};
#endif // HULLPLOTTER_H
