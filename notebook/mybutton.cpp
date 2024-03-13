#include "mybutton.h"

MyButton::MyButton(QWidget *parent) : QWidget(parent)
{
    pic.load(":/icon/o1.png");
    setFixedSize(pic.size());
    update();
}
void MyButton::leaveEvent(QEvent *event)
{
    pic.load(":/icon/o1.png");
    update();
}
void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    pic.load(":/icon/o3.png");
    update();
    emit clicked();
}
void MyButton::enterEvent(QEvent *event)
{
    pic.load(":/icon/o2.png");
    update();
}
void MyButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), pic);
}
