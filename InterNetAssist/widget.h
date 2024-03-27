#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QUdpSocket>
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
    QTcpSocket *client;
    QUdpSocket *Udp;
    QList<QHostAddress> address;
    void refreshClient();

private:
    Ui::Widget *ui;
private slots:
    void on_SelectProtocol(int index);
    void on_TCPserverOpenButton();
    void on_TCPclientconnect();
    void on_UDPOpenButton();
    void on_NewClient();
    void on_TcpServerReadyRead();
    void on_Send();
    void on_TcpClientClose();
    void on_TCPClientReadRedy();
    void on_UdpReadRedy();
};
#endif // WIDGET_H
