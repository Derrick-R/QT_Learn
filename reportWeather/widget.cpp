#include "widget.h"
#include "ui_widget.h"
#include "widget.h"
#include "widget.h"





Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayoutMain);
    setWindowFlag(Qt::FramelessWindowHint);
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::search_On);//点击搜索按钮触发槽函数
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Widget::search_On);//回车触发槽函数
    menu = new QMenu(this);
    menu->setStyleSheet("QMenu { color: white; }");
    QAction *closeAct = new QAction(QIcon(":/res/close.png"), tr("退出"), this);
    menu->addAction(closeAct);
    connect(closeAct, &QAction::triggered, [=](){
        this->close();
    });
    manager = new QNetworkAccessManager(this);
    QUrl url("http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=62959456&appsecret=zWWc5u2U");
    QNetworkRequest request(url);
    // 发起GET请求，获取返回的QNetworkReply对象指针
    manager->get(request);
    connect(manager, &QNetworkAccessManager::finished, this, &Widget::httpReply);


    mWeekList << ui->labeliqday0 << ui->labeliqday1
              << ui->labeliqday2 << ui->labeliqday3
              << ui->labeliqday4 << ui->labeliqday5;
    mDateList << ui->labeliqdate0 << ui->labeliqdate1
              << ui->labeliqdate2 << ui->labeliqdate3
              << ui->labeliqdate4 << ui->labeliqdate5;
    mIconList << ui->labelWeatherTypeIcon0 << ui->labelWeatherTypeIcon1
              << ui->labelWeatherTypeIcon2 << ui->labelWeatherTypeIcon3
              << ui->labelWeatherTypeIcon4 << ui->labelWeatherTypeIcon5;
    mWeaTypeList << ui->labelWeatherType0 << ui->labelWeatherType1
                 << ui->labelWeatherType2 << ui->labelWeatherType3
                 << ui->labelWeatherType4 << ui->labelWeatherType5;
    mAirqList << ui->labeliq0 << ui->labeliq1
              << ui->labeliq2 << ui->labeliq3
              << ui->labeliq4 << ui->labeliq5;
    mFxList << ui->labelWind0 << ui->labelWind1
            << ui->labelWind2 << ui->labelWind3
            << ui->labelWind4 << ui->labelWind5;
    mFlList << ui->labelWindLevel0 << ui->labelWindLevel1
            << ui->labelWindLevel2 << ui->labelWindLevel3
            << ui->labelWindLevel4 << ui->labelWindLevel5;

    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("undefined",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    ui->widget0402->installEventFilter(this);
    ui->widget0403->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::parseWeatherJsonData(QByteArray rawData)
{
    QJsonDocument jsonObj = QJsonDocument::fromJson(rawData);
    if(!jsonObj.isNull() && jsonObj.isObject())
    {
        QJsonObject rootObj = jsonObj.object();
        ui->labelCurrentDate->setText(rootObj["date"].toString() + " " + rootObj["week"].toString());//日期
        ui->labelCity->setText(rootObj["city"].toString());//城市
        ui->labelTmp->setText(rootObj["tem"].toString() + "℃");//温度
        ui->labelTmpRange->setText(rootObj["tem2"].toString() + "-" + rootObj["tem1"].toString() + "℃");//温度范围
        ui->labelWeatherType->setText(rootObj["wea"].toString());//天气类型
        ui->labelWeatherIcon->setPixmap(mTypeMap[rootObj["wea"].toString()]);
        ui->labelGanMao->setText(rootObj["air_tips"].toString());//tips
        ui->labelWind->setText(rootObj["win"].toString());
        ui->labelWindLevel->setText(rootObj["win_speed"].toString());
        ui->labelHumidityLevel->setText(rootObj["humidity"].toString());
        ui->labelPMLevel->setText(rootObj["air_pm25"].toString());
        ui->labelAirQualityLevel->setText(rootObj["air_level"].toString());
    }
}

void Widget::parseWeatherJsonDataNew(QByteArray rawData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData);
    if( !jsonDoc.isNull() && jsonDoc.isObject()){
        QJsonObject jsonRoot = jsonDoc.object();
        days[0].mCity = jsonRoot["city"].toString();
        days[0].mPm25 = jsonRoot["aqi"].toObject()["pm25"].toString();
        if( jsonRoot.contains("data") && jsonRoot["data"].isArray()){
            QJsonArray weaArray = jsonRoot["data"].toArray();
            for(int i = 0; i < weaArray.size(); i++){
                QJsonObject obj = weaArray[i].toObject();
                days[i].mDate = obj["date"].toString();
                days[i].mWeek = obj["week"].toString();
                days[i].mWeathType = obj["wea"].toString();
                days[i].mTemp = obj["tem"].toString();
                days[i].mTempLow = obj["tem2"].toString();
                days[i].mTempHigh = obj["tem1"].toString();
                days[i].mFx = obj["win"].toArray()[0].toString();
                days[i].mFl = obj["win_speed"].toString();
                days[i].mAirq = obj["air_level"].toString();
                days[i].mTips = obj["index"].toArray()[3].toObject()["desc"].toString();
                days[i].mHu = obj["humidity"].toString();
            }
        }
    }
    updateUI();
}
void Widget::updateUI()
{
    QPixmap pixmap;
    //解析日期
    ui->labelCurrentDate->setText(days[0].mDate+"  "+days[0].mWeek);
    //解析城市名称
    ui->labelCity->setText(days[0].mCity);
    //解析当前温度
    ui->labelTmp->setText(days[0].mTemp+"℃");
    ui->labelTmpRange->setText(days[0].mTempLow+"-"+days[0].mTempHigh+"℃");
    //解析天气类型
    ui->labelWeatherType->setText(days[0].mWeathType);
    ui->labelWeatherIcon->setPixmap(mTypeMap[days[0].mWeathType]);
    //感冒指数
    ui->labelGanMao->setText(days[0].mTips);
    //风向
    ui->labelWind->setText(days[0].mFx);
    //风力
    ui->labelWindLevel->setText(days[0].mFl);
    //PM2.5
    ui->labelPMLevel->setText(days[0].mPm25);
    //湿度
    ui->labelHumidityLevel->setText(days[0].mHu);
    //空气质量
    ui->labelAirQualityLevel->setText(days[0].mAirq);
    for(int i = 0; i < 6; i++){
        mWeekList[i]->setText(days[i].mWeek);
        mWeekList[0]->setText("今天");
        mWeekList[1]->setText("明天");
        mWeekList[2]->setText("后天");
        QStringList dayList = days[i].mDate.split("-");
        mDateList[i]->setText(dayList.at(1)+"-"+dayList.at(2));//2024-01-24

        //缩放图片大小和label大小能匹配
        int index = days[i].mWeathType.indexOf("转");
        if(index != -1){
            pixmap = mTypeMap[days[i].mWeathType.left(index)];
        }else{
            pixmap = mTypeMap[days[i].mWeathType];
        }
        pixmap = pixmap.scaled(mIconList[i]->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        mIconList[i]->setMaximumHeight(50);
        mIconList[i]->setMaximumWidth(ui->widget02->width()/6.5);

        mIconList[i]->setPixmap(pixmap);
        mWeaTypeList[i]->setText(days[i].mWeathType);
        QString airQ = days[i].mAirq;
        mAirqList[i]->setText(airQ);
        if( airQ == "优"){
            mAirqList[i]->setStyleSheet("background-color: rgb(150, 213, 32); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "良"){
            mAirqList[i]->setStyleSheet("background-color: rgb(208, 107, 39); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "轻度"){
            mAirqList[i]->setStyleSheet("background-color: rgb(255, 199, 199); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "中度"){
            mAirqList[i]->setStyleSheet("background-color: rgb(255, 17, 17); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        if( airQ == "重度"){
            mAirqList[i]->setStyleSheet("background-color: rgb(153, 0, 0); border-radius: 7px; color: rgb(230, 230, 230)");
        }
        mFxList[i]->setText(days[i].mFx);
        index = days[i].mFl.indexOf("转");
        if(index != -1){
            mFlList[i]->setText(days[i].mFl.left(index));
        }else{
            mFlList[i]->setText(days[i].mFl);
        }
    }
    update();
}
void Widget::httpReply(QNetworkReply *reply)
{
    int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//状态码 200为正常
    if (reply->error() == QNetworkReply::NoError) {
                // 获取返回的数据
                QByteArray responseData = reply->readAll();
                parseWeatherJsonDataNew(responseData);
            } else {
                // 输出错误信息
                QMessageBox msg;
                msg.setWindowTitle("error");
                msg.setText("网络请求失败 Code:"+QString::number(resCode));
                msg.setIcon(QMessageBox::Warning);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();

            }
    reply->deleteLater(); // 最后记得释放reply对象
}
QString getCityCodeFromJson(QString name){
    QFile file(":/citycode.json");
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData);
    if(jsonDoc.isArray()){
        QJsonArray citys = jsonDoc.array();
        for(QJsonValue value : citys){
            if(value.isObject()){
                if(name == value["city_name"].toString() || name+"市" == value["city_name"].toString()
                        || name+"区" == value["city_name"].toString() || name+"县" == value["city_name"].toString())
                    return value["city_code"].toString();
            }
        }
    }
    return NULL;
}


void Widget::search_On()
{
    QString cityName = ui->lineEdit->text();
    QString cityCode = getCityCodeFromJson(cityName);
    if(cityCode != NULL){
        QString url = "http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=62959456&appsecret=zWWc5u2U&cityid=" + cityCode;
        // 发起GET请求，获取返回的QNetworkReply对象指针
        manager->get(QNetworkRequest(QUrl(url)));
    }else{
        QMessageBox box;
        box.setWindowTitle("错误");
        box.setText("请输入正确的城市名");
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
    }

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        menu->exec(QCursor::pos());
    }else if(event->button() == Qt::LeftButton)
    {
        mOffset = event->pos();//获取点击时鼠标和页面的相对位置
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);//鼠标移动改变event->globalPos(),页面跟着移动，但是减去相对位置保持鼠标和页面相对位置不变
}


void Widget::drawTempLineHigh(){
    QPainter painter(ui->widget0402);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::yellow);
    int avg=0, sum=0;
    for (int i = 0; i < 6; i++) {
        sum += days[i].mTempHigh.toInt();
    }
    avg = sum/6;
    //定义6个点-温度
    QVector<QPoint> points;
    QPoint point;
    for(int i = 0; i < 6; i++)
    {
        point.setX(mAirqList[i]->x()+mAirqList[i]->width()/2);//x轴在空气质量的正中间
        point.setY(ui->widget0402->height()/2 - (days[i].mTempHigh.toInt() - avg)*3);//y轴根据温度平均值上下浮动
        points.push_back(point);
        if(i >0 && i <5)points.push_back(point);
        painter.drawEllipse(point, 4,4);
        painter.drawText(point.x()-10, point.y()-10, days[i].mTempHigh+"℃");

    }
    painter.drawLines(points);
}

void Widget::drawTempLineLow()
{
    QPainter painter(ui->widget0403);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(0,170,200));
    painter.setPen(QColor(0,170,200));
    int avg=0, sum=0;
    for (int i = 0; i < 6; i++) {
        sum += days[i].mTempLow.toInt();
    }
    avg = sum/6;
    //定义6个点-温度
    QVector<QPoint> points;
    QPoint point;
    for(int i = 0; i < 6; i++)
    {
        point.setX(mAirqList[i]->x()+mAirqList[i]->width()/2);//x轴在空气质量的正中间
        point.setY(ui->widget0403->height()/2 - (days[i].mTempLow.toInt() - avg)*3);//y轴根据温度平均值上下浮动
        points.push_back(point);
        if(i >0 && i <5)points.push_back(point);
        painter.drawEllipse(point, 4,4);
        painter.drawText(point.x()-10, point.y()-10, days[i].mTempLow+"℃");

    }
    painter.drawLines(points);
}
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget0402 && event->type() == QEvent::Paint)
    {
        drawTempLineHigh();
    }else if(watched == ui->widget0403 && event->type() == QEvent::Paint){
        drawTempLineLow();
    }
}

