#include "hullplotter.h"
#include "ui_hullplotter.h"

HullPlotter::HullPlotter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HullPlotter)
{
    ui->setupUi(this);

    double scale_factor = 0.12;

    bodyPlanScene = new QGraphicsScene(this);
    ui->body_GraphicsView->setScene(bodyPlanScene);
    ui->body_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->body_GraphicsView->scale(0.5, 0.5);

    breadthPlanScene = new QGraphicsScene(this);
    ui->breadth_GraphicsView->setScene(breadthPlanScene);
    ui->breadth_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->breadth_GraphicsView->scale(scale_factor, scale_factor);
    ui->breadth_GraphicsView->rotate(-90);

    sheerPlanScene = new QGraphicsScene(this);
    ui->sheer_GraphicsView->setScene(sheerPlanScene);
    ui->sheer_GraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->sheer_GraphicsView->scale(scale_factor, -scale_factor);
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

    bigfont.setPixelSize(50);
    reallybigfont.setPixelSize(300);

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
    Hull_qt hull;
    HullDataReader h;
    hull.stations.clear();
    h.read("test.dat");

    std::string file;
    file = "pointcloud_data/GA_" + std::to_string(generation_number.value()) + ".pc";

    //std::cout << "trying to read " << file << std::endl;
    h.read(file);
    h.write_hull_qt(hull);

    std::string textfile("textfiles/GA_" + std::to_string(generation_number.value()) + ".txt");
    updateText(textfile);

    //clock example
    using std::chrono::system_clock;
    system_clock::time_point today = system_clock::now();
    std::time_t tt;
    tt = system_clock::to_time_t ( today );
    //std:: cout << ctime(&tt) << std::endl;

    ui->time_label->setText(ctime(&tt));

    body_plan(hull);
    breadth_plan(hull);
    sheer_plan(hull);
}

void HullPlotter::updateText(std::string filename) {

    std::ifstream file(filename);
    std::string str;
    //std::string file_contents;
    QString file_contents;
    while (std::getline(file, str))
    {
      file_contents.push_back(str.c_str());
      file_contents.push_back('\n');
    }


    ui->textBrowser->setText(file_contents);
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
    }
    /*QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setPos(200,350);
    io->setPlainText("Front");
    io->setFont(bigfont);
    bodyPlanScene->addItem(io);*/
}

//looking from above
void HullPlotter::breadth_plan(Hull_qt& hull) {

    breadthPlanLines.clear();
    breadthPlanScene->clear();

    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.x, hull.stations[i].chine.z));
    }
    std::vector<QPointF> origin_points;
    for (int i = 0; i<5; ++i) {
        origin_points.push_back(QPointF(hull.stations[i].origin.x, hull.stations[i].origin.z));
    }
    std::vector<QPointF> beam_points;
    for (int i = 0; i<5; ++i) {
        beam_points.push_back(QPointF(hull.stations[i].beam.x, hull.stations[i].beam.z));
    }

    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), chinePen_thick));
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), keelPen_thick));
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(beam_points.begin()), second = beam_points.begin(); first!=beam_points.end(); ++first, ++second) {
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *second), sheerPen_thick));
        breadthPlanLines.push_back(breadthPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    //TODO: Plot stations

    /*QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setPos(500,50);
    io->setPlainText("Top");
    io->setFont(reallybigfont);
    breadthPlanScene->addItem(io);*/
}

//looking from the side
void HullPlotter::sheer_plan(Hull_qt& hull) {
    sheerPlanLines.clear();
    sheerPlanScene->clear();

    //painter.setPen(QColor(0,0,0));

    std::vector<QPointF> chine_points;
    for (int i = 0; i<5; ++i) {
        chine_points.push_back(QPointF(hull.stations[i].chine.y, hull.stations[i].chine.z));
    }
    std::vector<QPointF> origin_points;
    for (int i = 0; i<5; ++i) {
        origin_points.push_back(QPointF(hull.stations[i].origin.y, hull.stations[i].origin.z));
    }
    std::vector<QPointF> keel_points;
    for (int i = 0; i<5; ++i) {
        keel_points.push_back(QPointF(hull.stations[i].keel.y, hull.stations[i].keel.z));
    }

    for (auto first = ++(chine_points.begin()), second = chine_points.begin(); first!=chine_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), chinePen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(origin_points.begin()), second = origin_points.begin(); first!=origin_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), sheerPen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    for (auto first = ++(keel_points.begin()), second = keel_points.begin(); first!=keel_points.end(); ++first, ++second) {
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *second), keelPen_thick));
        sheerPlanLines.push_back(sheerPlanScene->addLine(QLineF(*first, *first), pointPen_thick));
    }

    /*QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setPos(200,50);
    io->setPlainText("Side");
    io->setFont(reallybigfont);
    sheerPlanScene->addItem(io);*/

    //TODO: Plot stations
}
