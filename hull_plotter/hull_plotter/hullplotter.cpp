#include "hullplotter.h"
#include "ui_hullplotter.h"

HullPlotter::HullPlotter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HullPlotter)
{
    ui->setupUi(this);
    //generation_number = Counter(this);
    //generation_number.setValue(0);

    bodyPlanScene = new QGraphicsScene(this);
    ui->body_GraphicsView->setScene(bodyPlanScene);
    ui->body_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->body_GraphicsView->scale(0.5, 0.5);

    breadthPlanScene = new QGraphicsScene(this);
    ui->breadth_GraphicsView->setScene(breadthPlanScene);
    ui->breadth_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->breadth_GraphicsView->scale(0.15, 0.15);
    ui->breadth_GraphicsView->rotate(-90);

    sheerPlanScene = new QGraphicsScene(this);
    ui->sheer_GraphicsView->setScene(sheerPlanScene);
    ui->sheer_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->sheer_GraphicsView->scale(0.15, -0.15);
    ui->sheer_GraphicsView->rotate(-90);

    sheerPen = QPen(Qt::green);
    sheerPen.setWidth(3);

    stationPen = QPen(Qt::red);
    stationPen.setWidth(3);

    chinePen = QPen(Qt::blue);
    chinePen.setWidth(3);

    keelPen = QPen(Qt::black);
    keelPen.setWidth(3);

    pointPen = QPen(Qt::black);
    pointPen.setWidth(20);

    defaultPen = QPen(Qt::black);
    keelPen.setWidth(3);

    sheerPen_thick = QPen(Qt::green);
    sheerPen_thick.setWidth(9);

    stationPen_thick = QPen(Qt::red);
    stationPen_thick.setWidth(9);

    chinePen_thick = QPen(Qt::blue);
    chinePen_thick.setWidth(9);

    keelPen_thick = QPen(Qt::black);
    keelPen_thick.setWidth(9);

    pointPen_thick = QPen(Qt::black);
    pointPen_thick.setWidth(300);

    defaultPen_thick = QPen(Qt::black);
    keelPen_thick.setWidth(9);

    //Set up a timer to update stuff regularily, see: http://doc.qt.io/qt-5/qtwidgets-widgets-analogclock-example.html
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    QObject::connect(timer, SIGNAL(timeout()), &generation_number, SLOT(increment()));
    timer->start(2000);
}

HullPlotter::~HullPlotter()
{
    delete ui;
}

void HullPlotter::paintEvent(QPaintEvent *)
{
//    ++counter;
//    bodyPlanLines[0]->setLine(counter, counter, 2*counter, 2*counter);

    Hull_qt hull;
    HullDataReader h;
    hull.stations.clear();
//    h.read("test.dat");

    std::string file;
    file = "pointcloud_data/GA_" + std::to_string(generation_number.value()) + ".pc";

    std::cout << "trying to read " << file << std::endl;
    h.read(file);
    h.write_hull_qt(hull);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //clock example
    using std::chrono::system_clock;
    system_clock::time_point today = system_clock::now();
    std::time_t tt;
    tt = system_clock::to_time_t ( today );
    //std::cout << "today is: " << ctime(&tt);

    painter.drawText(QRect(20,20,200,200),ctime(&tt));
    std:: cout << ctime(&tt) << std::endl;

    ui->time_label->setText(ctime(&tt));

    body_plan(hull);
    breadth_plan(hull);
    sheer_plan(hull, painter);
    painter.end();
}

//view from the front
void HullPlotter::body_plan(Hull_qt& hull) {

    bodyPlanLines.clear();
    bodyPlanScene->clear();

    for (auto& iter : hull.stations) {
        QPointF origin(iter.origin.x, iter.origin.y);
        QPointF beam(iter.beam.x, iter.beam.y);
        QPointF chine(iter.chine.x, iter.chine.y);
        QPointF keel(iter.keel.x, iter.keel.y);

        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(origin, beam), sheerPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(beam, chine), stationPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(chine, keel), stationPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(keel,origin), defaultPen)); //remove?

        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(keel, keel), pointPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(chine, chine), pointPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(beam, beam), pointPen));
        bodyPlanLines.push_back(bodyPlanScene->addLine(QLineF(origin, origin), pointPen));

//        ui->breadth_GraphicsView->scale(0.5, 0.5);

        /*for (auto iter : bodyPlanLines) {
            iter->setScale(0.5);
        }*/
    }
}

//looking from above
void HullPlotter::breadth_plan(Hull_qt& hull) {

    breadthPlanLines.clear();
    breadthPlanScene->clear();

    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.x, hull.stations[i].chine.z));
        breadthPlanScene->addEllipse(hull.stations[i].chine.x, hull.stations[i].chine.z, 100, 100);
    }
    std::vector<QPointF> origin_points;
    for (int i = 0; i<5; ++i) {
        origin_points.push_back(QPointF(hull.stations[i].origin.x, hull.stations[i].origin.z));
        breadthPlanScene->addEllipse(hull.stations[i].origin.x, hull.stations[i].origin.z, 100, 100);
    }
    std::vector<QPointF> beam_points;
    for (int i = 0; i<5; ++i) {
        beam_points.push_back(QPointF(hull.stations[i].beam.x, hull.stations[i].beam.z));
        breadthPlanScene->addEllipse(hull.stations[i].beam.x, hull.stations[i].beam.z, 100, 100);
    }

    /*QTransform transform;
    transform.translate(100, 400);
    transform.scale(0.2,0.2);
    transform.rotate(-90);
    painter.setTransform(transform);*/

    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), chinePen_thick));
        //breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), keelPen_thick));
        //breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(beam_points.begin()), second = beam_points.begin(); first!=beam_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), sheerPen_thick));
        //breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }



    /*for (auto iter : breadthPlanLines) {
        iter->setScale(0.2);
    }*/

    /*transform.scale(-1,1);
    painter.setTransform(transform);*/

    //previously everything was plotted again, under influence of the mirror transformation

    //TODO: Plot stations
}

//looking from the side
void HullPlotter::sheer_plan(Hull_qt& hull, QPainter& painter) {
    sheerPlanLines.clear();
    sheerPlanScene->clear();

    //painter.setPen(QColor(0,0,0));

    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.y, hull.stations[i].chine.z));
    }
    std::vector<QPointF> origin_points;
    for (int i = 4; i<5; ++i) {
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


    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), chinePen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), keelPen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(keel_points.begin()), second = keel_points.begin(); first!=keel_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), keelPen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

   /* painter.setPen(QColor(0,0,255));
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
    }*/


    //TODO: Plot stations
}
