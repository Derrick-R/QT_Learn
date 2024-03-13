#include "mytextedit.h"

/*自定义控件实现TextEdit的Crtl+鼠标滚轮放大缩小字体*/

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void MyTextEdit::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
                // 获取滚轮的滚动方向
                if (e->angleDelta().y() > 0) {
                    // 向上滚动，放大文字
                    zoomIn();
                } else if (e->angleDelta().y() < 0) {
                    // 向下滚动，缩小文字
                    zoomOut();
                }
                e->accept(); // 表示事件已经被处理
            } else {
                // 没有按下 Ctrl，调用基类的 wheelEvent 处理其他滚动事件
                QTextEdit::wheelEvent(e);
            }

}
