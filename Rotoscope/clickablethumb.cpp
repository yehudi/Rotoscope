#include "clickablethumb.h"

ClickableThumb::ClickableThumb() : QLabel(){

}

ClickableThumb::~ClickableThumb(){

}

void ClickableThumb::mousePressEvent(QMouseEvent * event){
    emit this->clicked(this->id);
}
