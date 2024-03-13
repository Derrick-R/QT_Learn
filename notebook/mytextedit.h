#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include "QTextEdit"
#include "QWheelEvent"
class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget *parent);

protected:
    void wheelEvent(QWheelEvent *e) override;

signals:

};

#endif // MYTEXTEDIT_H
