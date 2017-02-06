#ifndef ADAPHMIWIDGETSTATE_H
#define ADAPHMIWIDGETSTATE_H
#include <QMainWindow>

class adaphmiwidgetstate
{
public:
    adaphmiwidgetstate();
    bool getWidgetState();
    void setWidgetState(QWidget *adapHmiWidget,bool widgetState);
private:
    QWidget *adapHmiWidget;
    bool widgetState;
};
#endif // ADAPHMIWIDGETSTATE_H
