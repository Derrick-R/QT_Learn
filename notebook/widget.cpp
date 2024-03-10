#include "widget.h"
#include "ui_widget.h"

using namespace std;
Widget::Widget(QWidget *parent):QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    ui->widgetBottom->setLayout(ui->horizontalLayout);
    QObject::connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(btnOpen_clicked()));
    QObject::connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(btnSave_clicked()));
    QObject::connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose_clicked()));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
    QObject::connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(oncursorPositionChanged()));
    QObject::connect(this, SIGNAL(mysignal(int)), this, SLOT(myslot(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::btnOpen_clicked()
{
//    QFileDialog fileDialog;
//    fileDialog.setFileMode(QFileDialog::ExistingFile);//ExistingFiles 可选择多个文件
//    QStringList a;
//    a.push_back("*.txt");
//    a.push_back("*.jpg");
//    fileDialog.setNameFilters(a);
//    fileDialog.exec();
//    QStringList fileName = fileDialog.selectedFiles();
//    for (QString str : fileName) {
//        qDebug() << str;
//    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "D:/QT", tr("Text (*.txt *.doc *.docx)"));
    ui->textEdit->clear();
    file.setFileName(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    QTextStream in(&file);
    in.setCodec(ui->comboBox->currentText().toStdString().c_str());

    this->setWindowTitle(fileName);//设置窗体显示文件名

    while(!in.atEnd())
    {
        QString str = in.readLine();
        ui->textEdit->append(str);
    }
}

void Widget::btnSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "D:/QT/untitled.txt", tr("Text (*.txt *.doc *.docx)"));
    this->setWindowTitle(fileName);//设置窗体显示文件名
    file.setFileName(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    QTextStream out(&file);
    out.setCodec(ui->comboBox->currentText().toStdString().c_str());
    //写入文件
    out << ui->textEdit->toPlainText();
}

void Widget::btnClose_clicked()
{
    if(file.isOpen()){
        file.close();
        ui->textEdit->clear();
        this->setWindowTitle("My NoteNook");
    }
}

void Widget::onCurrentIndexChanged(int index)
{
    QString str = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    if(file.isOpen())
    {
        file.seek(0);
        QTextStream in(&file);
        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
        while(!in.atEnd())
        {
            QString str = in.readLine();
            ui->textEdit->append(str);
        }
    }
}

void Widget::oncursorPositionChanged()
{
    QTextCursor cursor= ui->textEdit->textCursor();
    char s[20];
    sprintf(s, "第%d行第%d列", cursor.blockNumber()+1, cursor.columnNumber()+1);
    ui->labelPosition->setText(s);
}

void Widget::myslot(int value)
{
    qDebug() << "mysignal test!!" << value;
}

