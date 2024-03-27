#include "widget.h"
#include "ui_widget.h"
#include "mycombobox.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);
    server = new QTcpServer(this);
    client = new QTcpSocket(this);
    Udp    = new QUdpSocket(this);
    ui->pushButtonSend->setEnabled(false);
    connect(ui->comboBoxProtocol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::on_SelectProtocol);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &Widget::on_Send);
    address = QNetworkInterface::allAddresses();
    on_SelectProtocol(0);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::refreshClient()
{
    ui->comboBoxDesAdddress->clear();
    QList<QTcpSocket *>socks = server->findChildren<QTcpSocket * >();
    for(QTcpSocket *sock: socks){

        QString tmp = sock->peerAddress().toString() + ":" + QString::number(sock->peerPort());
        ui->comboBoxDesAdddress->addItem(tmp);
    }
    if(socks.size())
        ui->comboBoxDesAdddress->addItem("all");
}

void Widget::on_SelectProtocol(int index)
{
    switch(index){
    case 0:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("打开");
        ui->labelIPAddress->setText("本地IP地址");
        ui->comboBoxIPAddress->setEditable(false);
        ui->labelDesAddress->show();
        ui->comboBoxDesAdddress->show();
        for(QHostAddress add: address){
            if(add.protocol() == QAbstractSocket::IPv4Protocol){
                ui->comboBoxIPAddress->addItem(add.toString());
            }
        }
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPclientconnect);
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_UDPOpenButton);
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPserverOpenButton);
        break;
    case 1:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("连接服务器");
        ui->labelIPAddress->setText("目的IP地址");
        ui->comboBoxIPAddress->setEditable(true);
        ui->comboBoxDesAdddress->setEditable(false);
        ui->labelDesAddress->hide();
        ui->comboBoxDesAdddress->hide();
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPserverOpenButton);
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_UDPOpenButton);
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPclientconnect);
        break;
    case 2:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("打开");
        ui->labelIPAddress->setText("本地IP地址");
        ui->comboBoxIPAddress->setEditable(false);
        ui->labelDesAddress->show();
        ui->comboBoxDesAdddress->show();
        ui->comboBoxDesAdddress->setEditable(true);
        for(QHostAddress add: address){
            if(add.protocol() == QAbstractSocket::IPv4Protocol){
                ui->comboBoxIPAddress->addItem(add.toString());
            }
        }
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPclientconnect);
        disconnect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPserverOpenButton);
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_UDPOpenButton);
        break;
    default:
        break;
    }
}

void Widget::on_TCPserverOpenButton()
{
    if(!server->isListening()){
        connect(server, &QTcpServer::newConnection, this, &Widget::on_NewClient);
        connect(ui->comboBoxDesAdddress, &MyComboBox::refresh, this, &Widget::refreshClient);
        QHostAddress addr(ui->comboBoxIPAddress->currentText());
        if(!server->listen(addr, ui->lineEditPort->text().toInt())){
            QMessageBox listenBox;
            listenBox.setWindowTitle("错误");
            listenBox.setText("端口号被占用");
            listenBox.exec();
            disconnect(server, &QTcpServer::newConnection, this, &Widget::on_NewClient);
            return;
        }
        ui->pushButtonOpen->setText("关闭");
        ui->comboBoxProtocol->setEnabled(false);
        ui->comboBoxIPAddress->setEnabled(false);
        ui->lineEditPort->setEnabled(false);
    }else{
        QList<QTcpSocket *>socks = server->findChildren<QTcpSocket * >();
        for(QTcpSocket *sock: socks){
            sock->close();
            sock->deleteLater();
        }
        server->close();
        ui->pushButtonOpen->setText("打开");
        ui->comboBoxProtocol->setEnabled(true);
        ui->comboBoxIPAddress->setEnabled(true);
        ui->lineEditPort->setEnabled(true);
        ui->pushButtonSend->setEnabled(false);
    }

}

void Widget::on_TCPclientconnect()
{
    static int tcpclient_connected = 0;
    if(!tcpclient_connected){
        client->connectToHost(ui->comboBoxIPAddress->currentText(), ui->lineEditPort->text().toInt());
        if(client->waitForConnected(1)) {
            ui->textEditMain->append("连接TCP服务器成功");
            tcpclient_connected = 1;
            ui->pushButtonOpen->setText("断开连接");
            ui->comboBoxProtocol->setEnabled(false);
            ui->comboBoxIPAddress->setEnabled(false);
            ui->lineEditPort->setEnabled(false);
            connect(client, &QTcpSocket::readyRead, this, &Widget::on_TCPClientReadRedy);
        } else {
            ui->textEditMain->append("连接失败,请检查IP地址和端口");
            tcpclient_connected = 0;
        }
    }else{
        client->close();
        ui->textEditMain->append("断开连接");
        ui->pushButtonOpen->setText("连接服务器");
        ui->comboBoxProtocol->setEnabled(true);
        ui->comboBoxIPAddress->setEnabled(true);
        ui->lineEditPort->setEnabled(true);
        ui->pushButtonSend->setEnabled(false);
        tcpclient_connected = 0;
    }


}

void Widget::on_UDPOpenButton()
{
    static bool udp_bind = false;
    if(!udp_bind){
        if(Udp->bind(QHostAddress(ui->comboBoxIPAddress->currentText()), ui->lineEditPort->text().toInt())){
            udp_bind = true;
            connect(Udp, &QUdpSocket::readyRead, this, &Widget::on_UdpReadRedy);
            ui->pushButtonOpen->setText("关闭");
            ui->pushButtonSend->setEnabled(true);
        }
    }else{
        Udp->close();
        udp_bind = false;
        ui->pushButtonOpen->setText("打开");
        ui->pushButtonSend->setEnabled(false);
    }

}

void Widget::on_NewClient()
{
    if(server->hasPendingConnections()){
        QTcpSocket *connection = server->nextPendingConnection();

        ui->textEditMain->insertPlainText("接受到一个客户端连接\n"
                                          "客户端地址："+connection->peerAddress().toString()+"\n"+
                                          "客户端端口号："+QString::number(connection->peerPort())+"\n");
        connect(connection, &QTcpSocket::readyRead, this, &Widget::on_TcpServerReadyRead);
        connect(connection, &QTcpSocket::disconnected, this, &Widget::on_TcpClientClose);
        if(!ui->pushButtonSend->isEnabled()){
            ui->pushButtonSend->setEnabled(true);
        }
    }
    refreshClient();
}

void Widget::on_TcpServerReadyRead()
{
    QTcpSocket *tempSock = qobject_cast<QTcpSocket *>(sender());
    QString recv = tempSock->readAll();
    ui->textEditMain->append("recv" + recv);
    ui->textEditMain->moveCursor(QTextCursor::End);
}
void Widget::on_TCPClientReadRedy()
{
    QString recv = client->readAll();
    ui->textEditMain->append("recv" + recv);
    ui->textEditMain->moveCursor(QTextCursor::End);
}

void Widget::on_UdpReadRedy()
{
    QString recv = Udp->readAll();
    ui->textEditMain->append("recv" + recv);
    ui->textEditMain->moveCursor(QTextCursor::End);
}
void Widget::on_Send()
{
    if(ui->comboBoxProtocol->currentIndex() == 0){
        QList<QTcpSocket *>socks = server->findChildren<QTcpSocket * >();
        if(ui->comboBoxDesAdddress->currentIndex() < socks.size())
        {
            QTcpSocket *sock = socks[ui->comboBoxDesAdddress->currentIndex()];
            sock->write(QByteArray(ui->textEditSend->toPlainText().toUtf8()));
        }else{
            for(QTcpSocket *tmp : socks){
                tmp->write(QByteArray(ui->textEditSend->toPlainText().toUtf8()));
            }
        }
        ui->textEditMain->append("Send:"+ui->textEditSend->toPlainText());
    }else if(ui->comboBoxProtocol->currentIndex() == 1){
        client->write(QByteArray(ui->textEditSend->toPlainText().toUtf8()));
        ui->textEditMain->append("Send:"+ui->textEditSend->toPlainText());
    }else{
        Udp->writeDatagram(QByteArray(ui->textEditSend->toPlainText().toUtf8()), QHostAddress(ui->comboBoxDesAdddress->currentText().split(":")[0]), ui->comboBoxDesAdddress->currentText().split(":")[1].toInt());
        ui->textEditMain->append("Send:"+ui->textEditSend->toPlainText());
    }

}

void Widget::on_TcpClientClose()
{
    QTcpSocket *tempSock = qobject_cast<QTcpSocket *>(sender());
    ui->textEditMain->insertPlainText("一个客户端断开连接连接\n"
                                      "客户端地址："+tempSock->peerAddress().toString()+"\n"+
                                      "客户端端口号："+QString::number(tempSock->peerPort())+"\n");
    tempSock->deleteLater();
    ui->comboBoxDesAdddress->removeItem(ui->comboBoxDesAdddress->findText(tempSock->peerAddress().toString() + ":" + QString::number(tempSock->peerPort())));
    if(ui->comboBoxDesAdddress->currentText() == "all")
    {
        ui->pushButtonSend->setEnabled(false);
        ui->comboBoxDesAdddress->clear();
    }
}

