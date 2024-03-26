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
#include <QLineEdit>
#include "mycombobox.h"
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
    void LoopSend();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    unsigned long long sendTotal;
    unsigned long long recvTotal;
    QString sendBack;
    QTimer *timer;
    QTimer *timeLoopSend;
    QString myTime;

    int buttonIndex;
    QList<QPushButton *> buttons;
    QList<QLineEdit *> lineEdits;
    QList<QCheckBox *> checkBoxs;

private slots:
    void on_CloseandOpen();
    void on_Send();
    void Serial_Rev();
    void on_ClearRev();
    void on_TimingSend(bool checked);
    void on_SaveRev();
    void on_Hex(bool checked);
    void on_HidePanel(bool checked);
    void on_HideHistory(bool checked);
    void refresh_Com();
    void on_pushButtonx();
    void on_LoopSend(bool checked);
    void on_Reset();
    void on_Save();
    void on_Load();

};
#endif // WIDGET_H
