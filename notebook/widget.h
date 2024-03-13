#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mybutton.h"
#include "iostream"
#include "QDebug"
#include "QFile"
#include "QFileDialog"
#include "QList"
#include "QMessageBox"
#include "QShortcut"
#include "QWheelEvent"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    QFile file;
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void Mybtn_clicked();
    void btnOpen_clicked();
    void btnSave_clicked();
    void btnClose_clicked();
    void onCurrentIndexChanged(int index);
    void oncursorPositionChanged();
    void myslot(int value);
signals:
    void mysignal(int value);
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
