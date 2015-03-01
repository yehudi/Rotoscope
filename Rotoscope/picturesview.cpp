#include "picturesview.h"
#include <QScrollArea>
#include <QDebug>

PicturesView::PicturesView() : QWidget(){
    this->fps = 6;
    this->containerLayout = new QHBoxLayout();
    this->setLayout(this->containerLayout);
    this->layout = new QHBoxLayout();
    this->layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    layout->setSpacing(0);
    layout->setMargin(0);
    this->widget = new QWidget();
    QScrollArea * area = new QScrollArea();
    area->setWidgetResizable(true);

    widget->setLayout(this->layout);
    this->containerLayout->addWidget(area);
    area->setWidget(widget);
    this->playTimer = new QTimer(this);
    connect(this->playTimer, SIGNAL(timeout()), this, SLOT(updatePlayer()));

}

PicturesView::PicturesView(QWidget * parent) : QWidget(parent){
    this->fps = 6;
    this->containerLayout = new QHBoxLayout();
    this->setLayout(this->containerLayout);
    this->layout = new QHBoxLayout();
    this->layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    layout->setSpacing(0);
    layout->setMargin(0);
    this->widget = new QWidget();
    area = new QScrollArea();
    area->setWidgetResizable(true);
    widget->setLayout(this->layout);

    this->containerLayout->addWidget(area);
    area->setWidget(widget);
    this->playTimer = new QTimer(this);
    connect(this->playTimer, SIGNAL(timeout()), this, SLOT(updatePlayer()));

}

void PicturesView::draw(){
    for (int i=0;i<this->thumbs.size(); ++i){
        this->layout->addWidget(this->thumbs[i]);
        this->thumbs[i]->id = i;
        connect(this->thumbs[i],SIGNAL(clicked(int)), this, SLOT(clickedPickture(int)));
    }
    this->selectPicture(0);
}

void PicturesView::selectPicture(int i){
    this->thumbs[this->zone_dessin->current_picture]->setStyleSheet("border: 0px solid blue");
    this->thumbs[i]->setStyleSheet("border: 2px solid blue");
    this->area->ensureWidgetVisible(this->thumbs[i],0,50);
    this->zone_dessin->loadImage(i);
}

void PicturesView::clickedPickture(int i){
    this->selectPicture(i);
    //this->thumbs[this->zone_dessin->current_picture]->setStyleSheet("border: 0px solid blue");
    //this->thumbs[i]->setStyleSheet("border: 2px solid blue");
    //emit this->askedPicture(i);
}



void PicturesView::nextPicture(){
    if(this->zone_dessin->current_picture < this->thumbs.size()-1){
        this->selectPicture(this->zone_dessin->current_picture +1);
    }
}

void PicturesView::prevPicture(){
    if(this->zone_dessin->current_picture > 0){
        this->selectPicture(this->zone_dessin->current_picture -1);
    }
}


void PicturesView::play(){
     this->playTimer->start(1000/this->fps);
}

void PicturesView::pause(){
    this->playTimer->stop();
}


void PicturesView::stop(){
     this->playTimer->stop();
     this->selectPicture(0);
}

void PicturesView::updatePlayer(){
    if(this->zone_dessin->current_picture == this->zone_dessin->paths.size()-1){
        this->selectPicture(0);
    }else{
        this->nextPicture();
    }
}

