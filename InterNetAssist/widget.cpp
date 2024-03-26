#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);
    ui->labelDesAddress->hide();
    ui->lineEditDesAddress->hide();
    server = new QTcpServer(this);
    connect(ui->comboBoxProtocol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::on_SelectProtocol);
    address = QNetworkInterface::allAddresses();
    on_SelectProtocol(0);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_SelectProtocol(int index)
{
    switch(index){
    case 0:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("打开");
        ui->labelIPAddress->setText("本地IP地址");
        ui->comboBoxIPAddress->setEditable(false);
        ui->labelDesAddress->hide();
        ui->lineEditDesAddress->hide();
        for(QHostAddress add: address){
            if(add.protocol() == QAbstractSocket::IPv4Protocol){
                ui->comboBoxIPAddress->addItem(add.toString());
            }
        }
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPserverOpenButton);
        break;
    case 1:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("连接服务器");
        ui->labelIPAddress->setText("目的IP地址");
        ui->comboBoxIPAddress->setEditable(true);
        ui->labelDesAddress->hide();
        ui->lineEditDesAddress->hide();
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_TCPclientconnect);
        break;
    case 2:
        ui->comboBoxIPAddress->clear();
        ui->pushButtonOpen->setText("打开");
        ui->labelIPAddress->setText("本地IP地址");
        ui->comboBoxIPAddress->setEditable(false);
        ui->labelDesAddress->show();
        ui->lineEditDesAddress->show();
        for(QHostAddress add: address){
            if(add.protocol() == QAbstractSocket::IPv4Protocol){
                ui->comboBoxIPAddress->addItem(add.toString());
            }
        }
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, &Widget::on_UDPOpenButton);
        break;
    default:
        break;
    }
}

void Widget::on_TCPserverOpenButton()
{
    if(!server->isListening() ){
        connect(server, &QTcpServer::newConnection, this, &Widget::on_NewClient);
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
    }else{
        server->close();
        ui->pushButtonOpen->setText("打开");
    }

}

void Widget::on_TCPclientconnect(bool checked)
{
    ui->pushButtonOpen->setText("断开");
}

void Widget::on_UDPOpenButton(bool checked)
{
    ui->pushButtonOpen->setText("关闭");
}

void Widget::on_NewClient()
{
    if(server->hasPendingConnections()){
        QTcpSocket *connection = server->nextPendingConnection();

        ui->textEditMain->insertPlainText("接受到一个客户端连接\n"
                                          "客户端地址："+connection->peerAddress().toString()+"\n"+
                                          "客户端端口号："+QString::number(connection->peerPort())+"\n");
        connect(connection, &QTcpSocket::readyRead, this, &Widget::on_TcpServerReadyRead);
    }
}

void Widget::on_TcpServerReadyRead()
{
    QTcpSocket *tempSock = qobject_cast<QTcpSocket *>(sender());
    QString recv = tempSock->readAll();
    ui->textEditMain->insertPlainText(recv);
    ui->textEditMain->moveCursor(QTextCursor::End);
}

