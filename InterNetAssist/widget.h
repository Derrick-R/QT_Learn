#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTcpServer *server;
    QList<QHostAddress> address;

private:
    Ui::Widget *ui;
private slots:
    void on_SelectProtocol(int index);
    void on_TCPserverOpenButton();
    void on_TCPclientconnect(bool checked);
    void on_UDPOpenButton(bool checked);
    void on_NewClient();
    void on_TcpServerReadyRead();
};
#endif // WIDGET_H
