#ifndef CLICKABLETHUMB_H
#define CLICKABLETHUMB_H

#include <QLabel>

class ClickableThumb : public QLabel{
    Q_OBJECT
public:
    int id;
    ClickableThumb();
    ~ClickableThumb();
signals:
    void clicked(int);
protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // CLICKABLETHUMB_H

