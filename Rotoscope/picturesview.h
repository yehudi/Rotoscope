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

    int fps;
    QZoneDessin * zone_dessin;
public slots:
    void clickedPickture(int);
    void nextPicture();
    void prevPicture();
    void play();
    void pause();
    void stop();
    void updatePlayer();
signals:
    void askedPicture(int);
private:
    QTimer * playTimer;
    QScrollArea * area;
    QLayout * layout;
    QLayout * containerLayout;
    QWidget * widget;
};

#endif // PICTURESVIEW_H

