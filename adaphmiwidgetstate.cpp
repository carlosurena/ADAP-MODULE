#include "adaphmiwidgetstate.h"

adaphmiwidgetstate::adaphmiwidgetstate()
{
}
void adaphmiwidgetstate::setWidgetState(QWidget *adapHmiWidget, bool widgetState){
    this->adapHmiWidget = adapHmiWidget;
    this->widgetState = widgetState;
}
bool adaphmiwidgetstate::getWidgetState(){
    return this->widgetState;
}
