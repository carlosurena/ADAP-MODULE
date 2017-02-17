#include "selector.h"
#include <QString>




Selector::Selector()
{

}


Selector::Selector(const QString s)
{
    SELECTED = s;
}



QString Selector::getSelector(){

    return SELECTED;
}
