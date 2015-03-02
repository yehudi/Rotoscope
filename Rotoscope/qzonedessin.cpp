
#include "qzonedessin.h"
#include "drawview.h"
#include <iostream>
#include <QtGui>
#include <QtCore>
#include <QDebug>

QZoneDessin::QZoneDessin(QWidget * parent) : QWidget(parent){
    this->m_image = NULL;
    m_actif = 0;
    this->current_picture = 0;
    this->afficherDecor = true;
    this->nombrePelures = 1;
    this->frequenceDessin = 1;
    this->drawed_picture = NULL;
    m_position = NULL;
    erase =false;
    this->acceptDrops();
    this->pen = QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap);
}


void QZoneDessin::loadImage(int number){
    int old =this->current_picture;
    if(number >= this->paths.size()){
         this->current_picture = this->paths.size()-1;
     }else{
        this->current_picture = number;
    }
     if(this->current_picture >= 0){
         if(this->m_image != NULL)
            delete this->m_image;
         if(this->drawed_picture != NULL){
             this->drawed_picture->save(this->paths[old]+".png");
             delete this->drawed_picture;
         }
         QImage temp(this->paths[this->current_picture]);
         this->drawed_picture = new QImage(this->paths[this->current_picture]+".png");
         this->m_image = new QImage(temp.scaled(640,480,Qt::KeepAspectRatio));
         this->update();
         this->setFixedSize(this->m_image->size());
     }
}


void QZoneDessin::setPen(QPen pen){
    this->pen = pen;
}


void QZoneDessin::resizeEvent(QResizeEvent * e)
{
    /*QImage * image = new QImage(e->size(),QImage::Format_ARGB32);
    image->fill(Qt::white);
    //copie de l'image
    QPainter p(image);
    p.drawImage(0,0,*m_image);
    p.drawImage(0,0,*(this->movie_pictures[this->current_picture]));
    p.drawImage(0,0,*(this->drawed_pictures[this->current_picture]));
    delete m_image;
    m_image = new QImage(*image);*/
}


void QZoneDessin::refresh()
{
    QPainter  p(this);
    p.fillRect(0,0,5000,5000, Qt::white);
    if(this->afficherDecor)
        p.drawImage(0,0,*this->m_image);
        //p.drawImage(0,0,*this->movie_pictures[this->current_picture]);

    int nbre = (this->nombrePelures -1 > this->current_picture)? this->current_picture: this->nombrePelures-1;
    qreal opacity;
    if (nbre == 0)
        opacity = 1.0;
    else
        opacity = 1.0/(float)(nbre+1);

    p.setOpacity(0);
    for(int i=nbre; i>0; --i){
        if(this->current_picture-i*this->frequenceDessin >= 0){
            p.setOpacity(p.opacity() + opacity);
            QImage img(this->paths[this->current_picture-i*this->frequenceDessin]+".png");
            p.drawImage(0,0,img);
        }
    }
    p.setOpacity(1);
    p.drawImage(0,0,*this->drawed_picture);
}


void QZoneDessin::paintEvent(QPaintEvent * e)
{
    this->refresh();
}

void QZoneDessin::mousePressEvent(QMouseEvent *event)
{
    m_position = new QPoint(event->pos() );
    m_actif = 1;
    QPainter paint(this->drawed_picture);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setBackgroundMode(Qt::TransparentMode);
    QPen pen = this->pen;
    QPoint position(event->pos());
    if(erase){
        pen.setColor(Qt::transparent);
        paint.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    }
    paint.setPen(pen);
    paint.drawPoint(QPointF(position));
    this->update();
}

void QZoneDessin::mouseMoveEvent(QMouseEvent *event){
    if (m_actif==1)
    {
        emit draw();
        QPainter paint(this->drawed_picture);
        paint.setRenderHint(QPainter::Antialiasing);
        QPen pen = this->pen;
        if(erase){
            pen.setColor(Qt::transparent);
            paint.setCompositionMode(QPainter::CompositionMode_SourceOut);
        }
        paint.setPen(pen);
        QPoint position(event->pos());
        paint.drawLine(QLine(*m_position,position));
        delete m_position;
        m_position = new QPoint(position);
        this->update();
    }
}


void QZoneDessin::mouseReleaseEvent(QMouseEvent *event)
{
    //std::cout << "relache" << std::endl;
    delete m_position;
    m_actif = 0;
}

void QZoneDessin::clearDessin()
{
    this->drawed_picture->fill(Qt::transparent);
    this->update();
}

