#ifndef QZONEDESSIN_H
#define QZONEDESSIN_H

#include <QtWidgets>
#include <QPen>
#include <drawview.h>

class QZoneDessin : public QWidget
{
    Q_OBJECT

public:
    std::vector<QImage *> movie_pictures;
    std::vector<QImage *> drawed_pictures;
    std::vector<QString> paths;
    int current_picture;

    bool afficherDecor;
    int nombrePelures;

    bool erase;

    QZoneDessin(QWidget * parent);
    void loadImage(int number);
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void clearDessin();
    void setPen(QPen pen);
    void refresh();
private:
    QGraphicsScene * scene;
    DrawView * view;
    QImage * m_image;
    QImage * drawed_picture;
    int m_actif;
    QPoint *m_position;
    QPen pen;


signals:
    void draw();


};

#endif // QZONEDESSIN_H

