#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "dayh.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    dayh days[7];
    QList<QLabel *> mDateList;
    QList<QLabel *> mWeekList;
    QList<QLabel *> mIconList;
    QList<QLabel *> mWeaTypeList;
    QList<QLabel *> mAirqList;
    QList<QLabel *> mFxList;
    QList<QLabel *> mFlList;


private:
    Ui::Widget *ui;
    QMenu *menu;
    QPoint mOffset;
    QNetworkAccessManager *manager;
    QMap<QString, QString> mTypeMap;
    void parseWeatherJsonData(QByteArray rawData);
    void parseWeatherJsonDataNew(QByteArray rawData);
    void updateUI();
public slots:
     void httpReply(QNetworkReply *reply);
     void search_On();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

};
#endif // WIDGET_H
