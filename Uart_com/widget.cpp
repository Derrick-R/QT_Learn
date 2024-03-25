#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayoutMain);
    updateTime();
    serialPort = new QSerialPort(this);
    timer = new QTimer(this);
    QTimer *timerDateTime = new QTimer(this);
    sendTotal = 0;
    recvTotal = 0;
    sendBack = "";
    //定时器信号
    connect(timer, &QTimer::timeout,[=](){
        on_Send();
    });
    connect(timerDateTime, &QTimer::timeout,[=](){
        updateTime();
    });
    timerDateTime->start(1000);
    //绑定按键信号与槽
    connect(ui->pushButtonCloseandOpen, &QPushButton::clicked, this, &Widget::on_CloseandOpen);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &Widget::on_Send);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &Widget::on_ClearRev);
    connect(ui->checkBoxTimingSend, &QCheckBox::toggled, this, &Widget::on_TimingSend);
    connect(ui->pushButtonSaveRev, &QPushButton::clicked, this, &Widget::on_SaveRev);
    connect(ui->checkBoxHex, &QPushButton::clicked, this, &Widget::on_Hex);
    connect(ui->pushButtonHidePanel, &QPushButton::clicked, this, &Widget::on_HidePanel);
    connect(ui->pushButtonHideHistory, &QPushButton::clicked, this, &Widget::on_HideHistory);
    //绑定combox自定义信号
    connect(ui->comboBoxCom, &MyComboBox::refresh, this, &Widget::refresh_Com);
    //绑定Serial准备接收信号
    connect(serialPort, &QSerialPort::readyRead, this, &Widget::Serial_Rev);
    //
    ui->comboBoxBaud->setCurrentIndex(6);
    ui->comboBoxDateBit->setCurrentIndex(3);
    ui->pushButtonSend->setEnabled(false);
    ui->checkBoxTimingSend->setEnabled(false);
    ui->revTextEdit->setReadOnly(true);
    ui->checkBoxHexsend->setEnabled(false);
    ui->checkBoxSendNewLine->setEnabled(false);
    //获取当前PC可用串口号
    QList<QSerialPortInfo> serials = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo serial : serials){
        ui->comboBoxCom->addItem(serial.portName());
    }
    //将多文本中的控件加入List
    for (int i=1; i <= 10; i++) {
        QString btnName = QString("pushButton_%1").arg(i);
        QString lineEditName = QString("lineEdit_%1").arg(i);
        QString checkBoxName = QString("checkBox_%1").arg(i);
        QPushButton* btn = findChild<QPushButton *>(btnName);
        QLineEdit *ledit = findChild<QLineEdit *>(lineEditName);
        QCheckBox *ckbox = findChild<QCheckBox *>(checkBoxName);
        btn->setProperty("ID", i);
        buttons.append(btn);lineEdits.append(ledit);checkBoxs.append(ckbox);
        connect(btn, &QPushButton::clicked, this, &Widget::on_pushButtonx);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    int year = currentTime.date().year();
    int month = currentTime.date().month();
    int day = currentTime.date().day();
    int hour = currentTime.time().hour();
    int min = currentTime.time().minute();
    int sec = currentTime.time().second();
    myTime = QString::number(year)+"-"+QString::number(month)+"-"
            +QString::number(day)+" "+QString::number(hour)+":"
            +QString::number(min)+":"+QString::number(sec);
    ui->labelTime->setText(myTime);
}

void Widget::on_CloseandOpen()
{
    if(!serialPort->isOpen()){
        serialPort->setPortName(ui->comboBoxCom->currentText());
        serialPort->setBaudRate(ui->comboBoxBaud->currentText().toInt());
        serialPort->setDataBits(QSerialPort::DataBits(ui->comboBoxDateBit->currentText().toInt()));
        switch (ui->comboBoxCheckBit->currentIndex()) {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
        case 3:
            serialPort->setParity(QSerialPort::OddParity);
            break;
        case 4:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
        default:
            serialPort->setParity(QSerialPort::UnknownParity);
            break;
        }
        if(ui->comboBoxStopBit->currentText().toFloat() == 1.5)
        {
            serialPort->setStopBits(QSerialPort::OneAndHalfStop);
        }else
        {
            serialPort->setStopBits(QSerialPort::StopBits(ui->comboBoxStopBit->currentText().toInt()));
        }
        if(ui->comboBoxStream->currentText() == "None"){
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
        }
        if(serialPort->open(QIODevice::ReadWrite)){
            ui->labelSendStatus->setText("Serial Open!");
            serialPort->clear(QSerialPort::Input);
            ui->pushButtonCloseandOpen->setText("关闭串口");
            ui->pushButtonSend->setEnabled(true);
            ui->checkBoxTimingSend->setEnabled(true);
            ui->groupBox->setEnabled(false);
            ui->checkBoxHexsend->setEnabled(true);
            ui->checkBoxSendNewLine->setEnabled(true);
        }else{
            QMessageBox serialOpenBox;
            const QString message = ui->comboBoxCom->currentText() + "打开失败";
            serialOpenBox.setWindowTitle("打开COM错误");
            serialOpenBox.setText(message);
            serialOpenBox.exec();
        }
    }else{
        emit ui->checkBoxTimingSend->toggled(false);
        ui->checkBoxTimingSend->setCheckState(Qt::Unchecked);
        serialPort->close();
        ui->pushButtonCloseandOpen->setText("打开串口");
        ui->pushButtonSend->setEnabled(false);
        ui->checkBoxTimingSend->setEnabled(false);
        ui->groupBox->setEnabled(true);
        ui->labelSendStatus->setText("Serial Close!");
        ui->checkBoxHexsend->setEnabled(false);
        ui->checkBoxSendNewLine->setEnabled(false);
    }

}

void Widget::on_Send()
{
    int endCnt = 0;
    QString sendData;
    //HEX发送
    if(ui->checkBoxHexsend->isChecked()){
        QString str = ui->lineEditSned->text();
        QByteArray bytestr = str.toUtf8();
        if(bytestr.size() % 2 !=0){
            ui->labelSendStatus->setText("Error Input!");
            return ;
        }
        for(char c : bytestr){
            if(!std::isxdigit(c)){
                ui->labelSendStatus->setText("Error Input!");
                return ;
            }
        }
        QByteArray byteSend =  QByteArray::fromHex(bytestr);
        //发送新行
        if(ui->checkBoxSendNewLine->isChecked()){
            byteSend.append("\r\n");
        }
        endCnt = serialPort->write(byteSend);
        sendData = byteSend;
    }else//正常发送
    {
        sendData = ui->lineEditSned->text();
        //发送新行
        if(ui->checkBoxSendNewLine->isChecked()){
            sendData += "\r\n";
        }
        endCnt = serialPort->write(sendData.toUtf8().data());
    }
    if(endCnt == -1)
    {
        ui->labelSendStatus->setText("Send Error!");
    }else{
        sendTotal += endCnt;
        ui->labelSendStatus->setText("Send OK!");
        const QString sendNums = "Send:" + QString::number(sendTotal);
        ui->labelSendNums->setText(sendNums);
        if(sendData != sendBack){
            ui->recordTextEdit->insertPlainText(sendData);
            sendBack = sendData;
        }
    }

}

void Widget::Serial_Rev()
{
    QString recv =  serialPort->readAll();
    if(recv != NULL)
    {
        //自动换行
        if(ui->checkBoxAutoLine->isChecked()){
            recv += "\r\n";
        }
        recvTotal += recv.length();
        if(ui->checkBoxHex->isChecked()){
            QString lastRecv;
            QByteArray byteRecv = recv.toUtf8();
            byteRecv = byteRecv.toHex();
            recv = QString::fromUtf8(byteRecv);
            for(int i = 0; i < recv.length(); i+=2) {
                // 添加当前两个字符到新字符串
                lastRecv += recv.mid(i, 2)+" ";
            }
            recv = lastRecv;
        }
        const QString sendNums = "Recv:" + QString::number(recvTotal);
        ui->labelRevNums->setText(sendNums);
        if(ui->checkBoxRevTime->checkState() == Qt::Checked){
            ui->revTextEdit->insertPlainText("["+myTime+"] "+recv);
        }else
            if(ui->checkBoxHex->isChecked()){
                ui->revTextEdit->insertPlainText(recv.toUpper());
            }else
                ui->revTextEdit->insertPlainText(recv);
    }

}

void Widget::on_ClearRev()
{
    ui->revTextEdit->clear();
    recvTotal = 0;
    const QString sendNums = "Recv:" + QString::number(recvTotal);
    ui->labelRevNums->setText(sendNums);
}

void Widget::on_TimingSend(bool checked)
{
    if (checked) {
        ui->pushButtonSend->setEnabled(false);
        ui->lineEditTimingMS->setEnabled(false);
        ui->lineEditSned->setEnabled(false);
        timer->start(ui->lineEditTimingMS->text().toUInt());
    } else{
        ui->pushButtonSend->setEnabled(true);
        ui->lineEditTimingMS->setEnabled(true);
        ui->lineEditSned->setEnabled(true);
        timer->stop();
    }
}

void Widget::on_SaveRev()
{
    QFile file;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "./untitled.txt", tr("Text (*.txt *.doc *.docx)"));
    this->setWindowTitle(fileName);//设置窗体显示文件名
    file.setFileName(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    //写入文件
    out << ui->revTextEdit->toPlainText();
    file.close();
}

void Widget::on_Hex(bool checked)
{
    if(checked){
        QString str = ui->revTextEdit->toPlainText();
        QByteArray bytestr = str.toUtf8();
        bytestr = bytestr.toHex();
        str = QString::fromUtf8(bytestr).toUpper();
        QString lastStr;
        for(int i = 0; i < str.length(); i+=2) {
                lastStr += str.mid(i, 2)+" ";
        }
        ui->revTextEdit->setText(lastStr);
        ui->checkBoxRevTime->setCheckState(Qt::Unchecked);
        ui->checkBoxRevTime->setEnabled(false);
    }else{
        QString str = ui->revTextEdit->toPlainText();
        QByteArray bytestr = str.toUtf8();
        bytestr = QByteArray::fromHex(bytestr);
        ui->revTextEdit->setText(QString::fromUtf8(bytestr));
        ui->checkBoxRevTime->setEnabled(true);
    }
    QTextCursor cursor = ui->revTextEdit->textCursor(); // 获取当前光标
    cursor.movePosition(QTextCursor::End);            // 移动光标到文本末尾
    ui->revTextEdit->setTextCursor(cursor);              // 更新光标位置
}

void Widget::on_HidePanel(bool checked)
{
    if(checked){
        ui->pushButtonHidePanel->setText("显示面板");
        ui->groupBoxRecord->hide();
    }else{
        ui->pushButtonHidePanel->setText("隐藏面板");
        ui->groupBoxRecord->show();
    }
}

void Widget::on_HideHistory(bool checked)
{
    if(checked){
        ui->pushButtonHideHistory->setText("显示历史");
        ui->groupBoxTexts->hide();
    }else{
        ui->pushButtonHideHistory->setText("隐藏历史");
        ui->groupBoxTexts->show();
    }
}
//刷新串口号
void Widget::refresh_Com()
{
    ui->comboBoxCom->clear();
    QList<QSerialPortInfo> serials = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo serial : serials){
        ui->comboBoxCom->addItem(serial.portName());
    }
    ui->labelSendStatus->setText("Serial refresh!");
}

void Widget::on_pushButtonx()
{
    int id = sender()->property("ID").toInt();
    qDebug() << id;
    ui->lineEditSned->setText(lineEdits[id-1]->text());
    on_Send();
}



