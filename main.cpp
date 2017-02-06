#include "adaphmimainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdapHmiMainWindow w;
    a.setApplicationName("Adaptive HMI");
    a.setOrganizationName("ServoRobot");
    a.setOrganizationDomain("servorobot.com");
    w.show();
    return a.exec();
}
