#include <QApplication>
 #include <QVBoxLayout>
 #include "dragwidget.h"
#include "selector.h"
#include <QPushButton>
#include <QLabel>
 int main(int argc, char *argv[])
 {
     Q_INIT_RESOURCE(draggableicons);

     QApplication app(argc, argv);

     QWidget mainWidget;
     QLabel *testLabel = new QLabel;

     testLabel->setText("YO");
    // QPushButton *deleteButton = new QPushButton("Delete");
     QVBoxLayout *verticalLayout = new QVBoxLayout;
     verticalLayout->addWidget(new DropWidget);
     verticalLayout->addWidget(new DragWidget);
     verticalLayout->addWidget(testLabel);
    // verticalLayout->addItem();
     mainWidget.setLayout(verticalLayout);
     mainWidget.setWindowTitle(QObject::tr("ADAP HMI MODULE"));
 #ifdef Q_OS_SYMBIAN
     mainWidget.showMaximized();
 #else
     mainWidget.show();
 #endif

     return app.exec();
 }
