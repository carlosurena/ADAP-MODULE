#ifndef ADAPHMIXMLCREATOR_H
#define ADAPHMIXMLCREATOR_H
#include <QString>
#include <QMap>
#include "adaphmisavelookuptable.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class adaphmixmlcreator
{
public:
    adaphmixmlcreator(QString adapHmiGeneratedOutput);
    ~adaphmixmlcreator();
    void createadaphmiXMLElements(QMap<QString,QString> inputOutputMap, QMap<adaphmisavelookuptable*,QString> looktableMap);
    XMLElement* generateSpreadSheetElements(QString name,QString equation,QString view,QString comments);
    XMLElement* generateLookUpTableElements(QString tagname, QString tagvalue);
    QString createLookTableDataforXML(QMap<adaphmisavelookuptable*,QString> looktableMap);
private:
    XMLDocument adaphmiXMLDoc;
    QString adapHmiGeneratedOutput;
};

#endif // ADAPHMIXMLCREATOR_H
