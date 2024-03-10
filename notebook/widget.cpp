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
    if(!file.isOpen())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "D:/QT/untitled.txt", tr("Text (*.txt *.doc *.docx)"));
        this->setWindowTitle(fileName);//设置窗体显示文件名
        file.setFileName(fileName);
        file.open(QFile::ReadWrite | QFile::Text);
    }
    QTextStream out(&file);
    out.setCodec(ui->comboBox->currentText().toStdString().c_str());
    //写入文件
    out << ui->textEdit->toPlainText();
}

void Widget::btnClose_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty())return;
    QMessageBox canselTrue;
    canselTrue.setText("The document has been modified\n"
                       "Do you want to save your changes?");
    canselTrue.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    canselTrue.setButtonText(QMessageBox::Save, "保存");
    canselTrue.setButtonText(QMessageBox::Discard, "丢弃");
    canselTrue.setButtonText(QMessageBox::Cancel, "取消");
    canselTrue.setDefaultButton(QMessageBox::Save);
    int ret = canselTrue.exec();
    switch(ret){
        case QMessageBox::Save:
            btnSave_clicked();
            break;
        case QMessageBox::Discard:
            if(file.isOpen()){
                file.close();
                this->setWindowTitle("My NoteNook");
            }
            ui->textEdit->clear();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
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

    //设置当前行高亮
    QList<QTextEdit::ExtraSelection> extraSelection;
    QTextEdit::ExtraSelection ext;
    //获取当前行
    ext.cursor = cursor;
    ext.cursor.clearSelection();
    //设置颜色
    QBrush qBrush(Qt::gray);
    ext.format.setBackground(qBrush);
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    extraSelection.push_back(ext);
    //设置
    ui->textEdit->setExtraSelections(extraSelection);
}

void Widget::myslot(int value)
{
    qDebug() << "mysignal test!!" << value;
}

