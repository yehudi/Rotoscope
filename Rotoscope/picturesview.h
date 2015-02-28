#ifndef PICTURESVIEW_H
#define PICTURESVIEW_H
#include <QWidget>
#include <vector>
#include <QLabel>
#include <QLayout>

#include "clickablethumb.h"
#include "qzonedessin.h"

class PicturesView : public QWidget{
    Q_OBJECT
public:
    std::vector<ClickableThumb *> thumbs;
    PicturesView();
    PicturesView(QWidget * parent);
    void selectPicture(int);
    void draw();

    QZoneDessin * zone_dessin;
public slots:
    void clickedPickture(int);
    void nextPicture();
    void prevPicture();
signals:
    void askedPicture(int);
private:
    QScrollArea * area;
    QLayout * layout;
    QLayout * containerLayout;
    QWidget * widget;
};

#endif // PICTURESVIEW_H

