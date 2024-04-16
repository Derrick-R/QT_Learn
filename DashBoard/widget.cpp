#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    currentvalue = 0;
    connect(timer, &QTimer::timeout, [=](){
        static int flag=0;
        if(flag == 0)
        {
            currentvalue++;
            if(currentvalue >= 50)flag=1;
        }else{
            currentvalue--;
            if(currentvalue<=0)flag=0;
        }

        update();
    });
    timer->start(50);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //底色修改
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    //
    painter.translate(rect().center());
    painter.save();
    //渐变
    QRadialGradient RadialGradient(0,0, 200);
    RadialGradient.setColorAt(0,QColor(255,0,0,50));
    RadialGradient.setColorAt(1,QColor(255,0,0,250));
    painter.setBrush(RadialGradient);

    painter.drawEllipse(QPoint(0,0), 250,250);

    painter.setPen(QPen(Qt::white, 3));
    painter.drawEllipse(QPoint(0,0), 50,50);
    //中间显示数值 Qt::AlignCente参数使文字居中
    painter.setFont(QFont("华文宋体",25));
    painter.drawText(QRect(-50,-50,100,100),Qt::AlignCenter, QString::number(currentvalue));

    //画刻度
    double ri=270.0/50;
    painter.rotate(-225);
    painter.drawLine(QPoint(230,0), QPoint(247,0));
    painter.setFont(QFont("华文宋体",15));
    painter.drawText(QPoint(205,7), QString::number(0));

    for (int i = 1; i <= 50; i++) {
        painter.rotate(ri);//旋转坐标画刻度
        if(i%10 == 0)
        {
            if(i < 30){
                painter.rotate(180);
                painter.drawText(QPoint(-220,7), QString::number(i));
                painter.rotate(-180);
            }else{
                painter.drawText(QPoint(205,7), QString::number(i));
            }
            painter.drawLine(QPoint(230,0), QPoint(247,0));

        }
        else
            painter.drawLine(QPoint(240,0), QPoint(247,0));
    }
    painter.rotate(90);
    //画指针
    painter.rotate(ri*currentvalue);
    painter.drawLine(QPoint(50,0), QPoint(195,0));
    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,128,0,158));
    painter.drawPie(QRect(-200,-200,400,400), 225*16, -ri*currentvalue*16);
}

