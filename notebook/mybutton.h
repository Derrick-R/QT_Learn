#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include "QPainter"
class MyButton : public QWidget
{
    Q_OBJECT
private:
    QPixmap pic;
public:
    explicit MyButton(QWidget *parent = nullptr);
protected:
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void clicked();

};

#endif // MYBUTTON_H
