#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include<QGraphicsView>
#include<QPixmap>
#include<QLabel>
#include<QSlider>
#include<QTextStream>
#include<QFile>
#include<QLayout>
#include<QMouseEvent>
class MapWidget : public QGraphicsView
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent = nullptr);
    void slotZoom(int);
    QPointF mapToMap(QPointF p);//坐标映射
    void readMap();
    ~MapWidget() override;
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPixmap map;
    qreal zoom;
    QLabel *viewCoord;
    QLabel *sceneCoord;
    QLabel *mapCoord;
    double x1,y1;
    double x2,y2;
};
#endif // MAPWIDGET_H
