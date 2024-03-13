#include "widget.h"
#include "ui_widget.h"

using namespace std;
Widget::Widget(QWidget *parent):QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    ui->widgetBottom->setLayout(ui->horizontalLayout);
    //快捷键
    QShortcut *shortCutSave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Open")), this);
    QShortcut *shortCutOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Save")), this);
    QShortcut *shortCutZoomIn = new QShortcut(QKeySequence(tr("Ctrl+Shift+=", "File|ZoomIn")), this);
    QShortcut *shortCutZoomOut = new QShortcut(QKeySequence(tr("Ctrl+Shift+-", "File|ZoomOut")), this);
    connect(shortCutSave, &QShortcut::activated, [=](){//connet绑定槽函数lamda表达式写法
        btnSave_clicked();
    });
    connect(shortCutOpen, &QShortcut::activated, [=](){
        btnOpen_clicked();
    });
    connect(shortCutZoomIn, &QShortcut::activated, [=](){
        //内置函数
        //ui->textEdit->zoomIn();
        //手动实现
        QFont font = ui->textEdit->font();
        font.setPointSize(font.pointSize()+1);
        ui->textEdit->setFont(font);
    });
    connect(shortCutZoomOut, &QShortcut::activated, [=](){
        //内置函数
        //ui->textEdit->zoomOut();
        //手动实现
        QFont font = ui->textEdit->font();
        if(font.pointSize() <= 0)return;
        font.setPointSize(font.pointSize()-1);
        ui->textEdit->setFont(font);
    });
    QObject::connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(btnOpen_clicked()));
    QObject::connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(btnSave_clicked()));
    QObject::connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnClose_clicked()));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
    QObject::connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(oncursorPositionChanged()));
    QObject::connect(this, SIGNAL(mysignal(int)), this, SLOT(myslot(int)));

    //QObject::connect(ui->Mybnt, SIGNAL(clicked()), this, SLOT(Mybtn_clicked()));//自定义按键绑定信号与槽函数
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox leaveBox;
    leaveBox.setText("Are you sure close the notebook?");
    leaveBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    leaveBox.setDefaultButton(QMessageBox::No);
    switch(leaveBox.exec()){
    case QMessageBox::Yes:
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    default:
        break;
    }
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


void Widget::Mybtn_clicked()
{
    qDebug() << "Mybtn_clicked test!!";
}
