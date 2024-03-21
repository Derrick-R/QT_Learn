#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QtDebug>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void updateTime();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    unsigned long long sendTotal;
    unsigned long long recvTotal;
    QString sendBack;
    QTimer *timer;
    QString myTime;

private slots:
    void on_CloseandOpen();
    void on_Send();
    void Serial_Rev();
    void on_ClearRev();
    void on_TimingSend(bool checked);
    void on_SaveRev();
    void on_Hex(bool checked);

};
#endif // WIDGET_H
