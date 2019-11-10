#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    readMap();
    zoom=50; //初始缩放在值
    int width = map.width();
    int height = map.height();
    QGraphicsScene *scene = new QGraphicsScene(this);

    //限定场景的显示区域为地图的大小
    //注意 graphicsscene的坐标原点在中心
    scene->setSceneRect(-width/2,-height/2,width,height);
    setScene(scene);
    setCacheMode(CacheBackground);

    //用于地图缩放的滑动条
    QSlider *slider = new QSlider;
    slider->setOrientation(Qt::Vertical);//垂直缩放条
    slider->setRange(1,100);
    slider->setTickInterval(10);//设置刻度
    slider->setValue(50);
    connect(slider,&QSlider::valueChanged,this,&MapWidget::slotZoom);//连接缩放slider与缩放函数

    QLabel *zoominLabel = new QLabel;
    zoominLabel->setScaledContents(true);
    zoominLabel->setPixmap(QPixmap("zoomin.png"));//图像原大小填充图标
    QLabel *zoomoutLabel = new QLabel;
    zoomoutLabel->setScaledContents(true);
    zoomoutLabel->setPixmap(QPixmap("zoomout.png"));


    //坐标值显示区 scene view以及map上的同一点在不同坐标系下的坐标
    QLabel *label1 = new QLabel(tr("GraphicsView:"));
    viewCoord = new QLabel;
    QLabel *label2 = new QLabel(tr("GraphicsScene:"));
    sceneCoord = new QLabel;
    QLabel *label3 = new QLabel(tr("map:"));
    mapCoord = new QLabel;

    //坐标显示区布局
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(label1,0,0);
    gridLayout->addWidget(viewCoord,0,1);
    gridLayout->addWidget(label2,1,0);
    gridLayout->addWidget(sceneCoord,1,1);
    gridLayout->addWidget(label3,2,0);
    gridLayout->addWidget(mapCoord,2,1);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    QFrame *coordFrame = new QFrame;
    coordFrame->setLayout(gridLayout);
    //缩放控制子布局
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(zoominLabel);
    zoomLayout->addWidget(slider);
    zoomLayout->addWidget(zoomoutLabel);
    //坐标显示区域布局
    QVBoxLayout *coordLayout = new QVBoxLayout;
    coordLayout->addWidget(coordFrame);
    coordLayout->addStretch();
    //主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(zoomLayout);
    mainLayout->addLayout(coordLayout);
    mainLayout->addStretch();
    mainLayout->setMargin(30);
    mainLayout->setSpacing(10);
    setLayout(mainLayout);
    setWindowTitle("Map Widget");
    setMinimumSize(600,400);
}

MapWidget::~MapWidget()
{
}
void MapWidget::readMap()
{
    QString mapName;
    QFile mapfile("maps.txt");
    bool result = mapfile.open(QIODevice::ReadOnly);
    if(result == true)
    {
        QTextStream ts(&mapfile);
        while(!ts.atEnd())
        {
            ts>>mapName;
            ts>>x1>>y1>>x2>>y2;
        }
        map.load(mapName);
    }
}

void MapWidget::slotZoom(int value)
{
    qreal s;//缩放比例
    if(value > zoom)//放大
    {
        s = pow(1.01,(value-zoom));
    }
    else
    {
        s = pow(1/1.01,(zoom-value));
    }
    //进行缩放
    scale(s,s);
    //重新设定zoom值
    zoom = value;
}
void MapWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(int(sceneRect().left()),int(sceneRect().top()),map);
}
QPointF MapWidget::mapToMap(QPointF p) //场景坐标 -- 经纬度（地图坐标）
{
    QPointF latLon;
    qreal w =sceneRect().width();
    qreal h =sceneRect().height();
    qreal lon = y1-((h/2+p.y())*abs(y1-y2)/h);
    qreal lat = x1+((w/2+p.x())*abs(x1-x2)/w);
    latLon.setX(lat);
    latLon.setY(lon);
    return latLon;
}
void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //QGraphicsView 坐标
    QPoint viewPoint = event->pos();
    viewCoord->setText(QString::number(viewPoint.x())+","+QString::number(viewPoint.y()));

    //QGraphicsScene 坐标
    QPointF scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::number(scenePoint.x())+","+QString::number(scenePoint.y()));

    //地图坐标(经、纬度值)
    QPointF latLon = mapToMap(scenePoint);
    mapCoord->setText(QString::number(latLon.x())+","+QString::number(latLon.y()));
}
