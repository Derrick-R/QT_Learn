#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "iostream"
#include "QDebug"
#include "QFile"
#include "QFileDialog"
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
private slots:
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
