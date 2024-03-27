#include "mycombobox.h"


MyComboBox::MyComboBox(QWidget *parent) : QComboBox(parent)
{

}

void MyComboBox::mousePressEvent(QMouseEvent *e)
{
    if(e->MouseButtonPress){
        emit refresh();
    }
    QComboBox::mousePressEvent(e);
}
