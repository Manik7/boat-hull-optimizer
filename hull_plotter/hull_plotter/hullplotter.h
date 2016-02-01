#ifndef HULLPLOTTER_H
#define HULLPLOTTER_H

#include <QMainWindow>
#include <QPainter>
#include <QTransform>

#include "Station_qt.h"
#include "Hull_qt.h"

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

protected:
  void paintEvent(QPaintEvent *);

};
#endif // HULLPLOTTER_H
