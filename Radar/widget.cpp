#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        startAngle++;
        if(startAngle == 360)startAngle=0;
        update();
    });
    timer->start(20);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::green, 3);
    QBrush brush(Qt::black);
    painter.setBrush(brush);
    painter.drawRect(rect());
    painter.setBrush(Qt::NoBrush);

    painter.setRenderHint(QPainter::Antialiasing, true);//抗锯齿
    painter.setPen(pen);
    int ri =height()/2/7;
    for(int i=1; i <= 7; i++)
    {
        painter.drawEllipse(rect().center(), ri*i, ri*i);
    }
    painter.drawLine(rect().center().x()-ri*7, height()/2, rect().center().x()+ri*7, height()/2);
    painter.drawLine(width()/2, 7, width()/2, height()-7);
    QRect newRect(rect().center().x()-ri*7, rect().center().y()-ri*7, ri*7*2, ri*7*2);
    QConicalGradient ConicalGradient(newRect.center(), -startAngle);
    ConicalGradient.setColorAt(0,QColor(0,255,0,200));
    ConicalGradient.setColorAt(0.1,QColor(0,255,0,100));
    ConicalGradient.setColorAt(0.2,QColor(0,255,0,0));
    ConicalGradient.setColorAt(1,QColor(0,255,0,0));
    painter.setBrush(ConicalGradient);
    painter.setPen(Qt::NoPen);
    painter.drawPie(newRect, -startAngle*16, 70*16);//(指定图形内,起始角度(正负代表逆顺),扇形大小(弧度)正负也代表逆顺)
}
