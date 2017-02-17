#include <QtGui>
#include <QTimer>
 #include "DragWidget.h"
#include <QPainterPathStroker>
#include <QString>
#include <QtDebug>
 DropWidget::DropWidget(QWidget *parent)
     : QFrame(parent)
 {
 #if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
 #else
     setMinimumSize(400, 500);
 #endif
     setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
     setAcceptDrops(true);



 }

 void DropWidget::dragEnterEvent(QDragEnterEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 void DropWidget::dragMoveEvent(QDragMoveEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 //When a QLabel Pixmap item is dropped onto the current (top) widget, draw a new icon in that position.
 void DropWidget::dropEvent(QDropEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         QPixmap pixmap;
         QPoint offset;
         dataStream >> pixmap >> offset;

         QLabel *newIcon = new QLabel(this);
         newIcon->setPixmap(pixmap);
         newIcon->move(event->pos() - offset);
         newIcon->show();
         newIcon->setAttribute(Qt::WA_DeleteOnClose);


       //  labelName = DragWidget::labelName;

         QString objName = labelName.SELECTED;
         qDebug() << labelName.SELECTED;
         int num = labelName.test69;
         qDebug() << num;

         newIcon->setObjectName(objName);





         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 void DropWidget::mousePressEvent(QMouseEvent *event)
 {
     QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
     if (!child)
         return;

    if(event->button() == Qt::RightButton){
        child->hide();
        return;
    }



    if(event->button() == Qt::MiddleButton){
        QString childName = child->objectName();
        int x = QString::compare(childName, "icon1", Qt::CaseInsensitive);
        qDebug() << childName;
        if(x==0){
        child->setPixmap(QPixmap(":/images/icon2.png"));
        }else{
            child->hide();
        }
        return;
    }

     QPixmap pixmap = *child->pixmap();
     QByteArray itemData;
     QDataStream dataStream(&itemData, QIODevice::WriteOnly);
     dataStream << pixmap << QPoint(event->pos() - child->pos());

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("application/x-dnditemdata", itemData);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setPixmap(pixmap);
     drag->setHotSpot(event->pos() - child->pos());

     QPixmap tempPixmap = pixmap;
     QPainter painter;
     painter.begin(&tempPixmap);
     painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
     painter.end();

     child->setPixmap(tempPixmap);


     if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
         child->close();
     else {
         child->show();
         child->setPixmap(pixmap);
     }

 }

