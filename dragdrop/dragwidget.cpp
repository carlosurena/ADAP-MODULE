#include <QtGui>

 #include "dragwidget.h"
#include "selector.cpp"
 DragWidget::DragWidget(QWidget *parent)
     : QFrame(parent)
 {
 #if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
 #else
     setMinimumSize(900, 120);
 #endif
     setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
     setAcceptDrops(false);

     QLabel *testIcon1 = new QLabel(this);
     testIcon1->setPixmap(QPixmap(":/images/areaIcon.png"));
     testIcon1->move(10, 10);
     testIcon1->show();
     testIcon1->setAttribute(Qt::WA_DeleteOnClose);
     testIcon1->setObjectName("icon1");

     QLabel *testIcon2 = new QLabel(this);
     testIcon2->setPixmap(QPixmap(":/images/gapIcon.png"));
     testIcon2->move(100, 10);
     testIcon2->show();
     testIcon2->setAttribute(Qt::WA_DeleteOnClose);
     testIcon2->setObjectName("icon2");

     QLabel *testIcon3 = new QLabel(this);
     testIcon3->setPixmap(QPixmap(":/images/mismatchIcon.png"));
     testIcon3->move(200, 10);
     testIcon3->show();
     testIcon3->setAttribute(Qt::WA_DeleteOnClose);
     testIcon3->setObjectName("icon3");



 }

 void DragWidget::dragEnterEvent(QDragEnterEvent *event)
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

 void DragWidget::dragMoveEvent(QDragMoveEvent *event)
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

 void DragWidget::dropEvent(QDropEvent *event)
 {
      //  if(){

        //}else{
         event->ignore();
 //       }
 }

 void DragWidget::mousePressEvent(QMouseEvent *event)
 {
     QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
     if (!child)
         return;

     QPixmap pixmap = *child->pixmap();

    //gives a name to the icon and passes it to the Selector class
     QString childName = child->objectName();
     qDebug() << childName;
     labelName.SELECTED = childName;
     labelName.test69= 69;

     QString testName = labelName.SELECTED;
     qDebug() << labelName.test69;


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


