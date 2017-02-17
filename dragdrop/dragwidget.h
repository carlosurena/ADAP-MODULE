#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>

class QDragEnterEvent;
class QDropEvent;

class Selector : public QFrame
{
public:
    Selector();
    Selector(QString);
    QString getSelector();
    QString SELECTED;
    int test69;

};

class DragWidget : public QFrame
{
public:
    DragWidget(QWidget *parent=0);
      Selector labelName;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

};

class DropWidget : public QFrame
{
public:
    DropWidget(QWidget *parent=0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    Selector labelName;
};




#endif // DRAGWIDGET_H
