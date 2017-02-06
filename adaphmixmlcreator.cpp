#include "adaphmixmlcreator.h"
#include "tinyxml2.h"
#include <QMap>
#include <QSet>
#include <adaphmisavelookuptable.h>
using namespace tinyxml2;

adaphmixmlcreator::adaphmixmlcreator(QString adapHmiGeneratedOutput){
    this->adapHmiGeneratedOutput = adapHmiGeneratedOutput;
}
adaphmixmlcreator::~adaphmixmlcreator(){
    adaphmiXMLDoc.Clear();
}
void adaphmixmlcreator::createadaphmiXMLElements(QMap<QString,QString> inputOutputMap, QMap<adaphmisavelookuptable*,QString> looktableMap)
{
    XMLDeclaration* adaphmiXMLdec = adaphmiXMLDoc.NewDeclaration();
    adaphmiXMLDoc.InsertFirstChild(adaphmiXMLdec);
    XMLElement* adaphmiRootXMLElements = adaphmiXMLDoc.NewElement("ADAPSettings");
    adaphmiRootXMLElements->SetAttribute("version","1.0");
    XMLElement* adaphmiXMLElements = adaphmiXMLDoc.NewElement("Enabled");
    adaphmiXMLElements->SetText("Yes");
    adaphmiRootXMLElements->InsertEndChild(adaphmiXMLElements);
    adaphmiXMLDoc.InsertEndChild(adaphmiRootXMLElements);
    adaphmiXMLElements = adaphmiXMLDoc.NewElement("SpreadSheet");
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("count","if(UnmuteTrigger==1,0,connt+1)","Yes","Execution counter (resets at laserunmute)"));
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("c0","if(connt&gt;0,jump(end_c0),0)","No","Conditional block for variable initialization"));
    if(inputOutputMap.contains("OP_CURRENT")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("I_lut",inputOutputMap.value("INPUT_CURRENT"),"No","Nominal current (amperes)"));
    }
    if(inputOutputMap.contains("OP_TRAVELSPEED")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("TS_lut",inputOutputMap.value("INPUT_TRAVELSPEED"),"No","Nominal travel speed (cm/min)"));
    }
    if(inputOutputMap.contains("OP_VOLTAGE")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("v_lut",inputOutputMap.value("INPUT_VOLTAGE"),"No","Nominal voltage (volts)"));
    }
    if(inputOutputMap.contains("OP_WIREFEED")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("WF_lut",inputOutputMap.value("INPUT_WIREFEED"),"No","Nominal wire feed (cm)"));
    }
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("c0","1","No","Label indicating end of conditional block"));
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("c1","if(notmatched&gt;0,jump(end_c1),0)","No","Skip table if vision is in NO MATCH status"));
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("ss1","push(area,s0)","No","Prepare stack for filtering of area signal"));
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("area_f","average(s0,50)","No","Average area"));
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("lut1","selectrow(area_f,0,0)","No","Select a row in lut based on filtered area"));
    int outputPosition = 0;
    if(inputOutputMap.contains("OP_CURRENT")){
        outputPosition++;
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("I_lut2","assign(I_lut,getcolumn("+QString::number(outputPosition)+"))","Yes","Select column "+QString::number(outputPosition)+" (assuming this is Current)"));
    }
    if(inputOutputMap.contains("OP_TRAVELSPEED")){
        outputPosition++;
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("TS_lut2","assign(TS_lut,getcolumn("+QString::number(outputPosition)+"))","Yes","Select column "+QString::number(outputPosition)+" (assuming this is Travel speed)"));
    }
    if(inputOutputMap.contains("OP_VOLTAGE")){
        outputPosition++;
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("v_lut2","assign(V_lut,getcolumn("+QString::number(outputPosition)+"))","Yes","Select column "+QString::number(outputPosition)+" (assuming this is Voltage)"));
    }
    if(inputOutputMap.contains("OP_WIREFEED")){
        outputPosition++;
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("WF_lut2","assign(WF_lut,getcolumn("+QString::number(outputPosition)+"))","Yes","Select column "+QString::number(outputPosition)+" (assuming this is Wire feed)"));
    }
    adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("end_c1","1","No","Label indicating end of conditional block"));
    if(inputOutputMap.contains("OP_CURRENT")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("current","I_lut","Yes","Assign output for current"));
    }
    if(inputOutputMap.contains("OP_TRAVELSPEED")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("travel","TS_lut","Yes","Assign output for travel speed"));
    }
    if(inputOutputMap.contains("OP_VOLTAGE")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("voltage","V_lut","Yes","Assign output for voltage"));
    }
    if(inputOutputMap.contains("OP_WIREFEED")){
        adaphmiXMLElements->InsertEndChild(generateSpreadSheetElements("wire","WF_lut","Yes","Assign output for wire feed"));
    }
    adaphmiRootXMLElements->InsertEndChild(adaphmiXMLElements);
    adaphmiXMLElements = adaphmiXMLDoc.NewElement("LookUpTable");
    XMLElement* adaphmiInnerXMLElements = adaphmiXMLDoc.NewElement("Columns");
    foreach(QString key, inputOutputMap.keys()){
        if(key.contains("IP_")){
            if(inputOutputMap.value(key) == "YES"){
                adaphmiInnerXMLElements->InsertEndChild(generateLookUpTableElements("Key",key.mid(3,key.length()-3)));
            }
        }
        if(key.contains("OP_")){
            if(inputOutputMap.value(key) == "YES"){
                adaphmiInnerXMLElements->InsertEndChild(generateLookUpTableElements("Value",key.mid(3,key.length()-3)));
            }
        }
    }
    adaphmiXMLElements->InsertEndChild(adaphmiInnerXMLElements);
    adaphmiInnerXMLElements = adaphmiXMLDoc.NewElement("Table");
    adaphmiInnerXMLElements->SetText(adaphmixmlcreator::createLookTableDataforXML(looktableMap).toLatin1().data());
    adaphmiXMLElements->InsertEndChild(adaphmiInnerXMLElements);
    adaphmiRootXMLElements->InsertEndChild(adaphmiXMLElements);
    adaphmiXMLElements = adaphmiXMLDoc.NewElement("DefaultValues");
    adaphmiXMLElements->SetText("\n    ");
    adaphmiRootXMLElements->InsertEndChild(adaphmiXMLElements);
    adaphmiXMLDoc.InsertEndChild(adaphmiRootXMLElements);
    adaphmiXMLDoc.SaveFile(adapHmiGeneratedOutput.toLatin1().data());
}

XMLElement* adaphmixmlcreator::generateSpreadSheetElements(QString name, QString equation, QString view, QString comments)
{
        XMLElement* adaphmiInnerXMLElements = adaphmiXMLDoc.NewElement("Row");
        XMLElement* adaphmiRowXMLElements = adaphmiXMLDoc.NewElement("Name");
        adaphmiRowXMLElements->SetText(name.toLatin1().data());
        adaphmiInnerXMLElements->InsertEndChild(adaphmiRowXMLElements);
        adaphmiRowXMLElements = adaphmiXMLDoc.NewElement("Equation");
        adaphmiRowXMLElements->SetText(equation.toLatin1().data());
        adaphmiInnerXMLElements->InsertEndChild(adaphmiRowXMLElements);
        adaphmiRowXMLElements = adaphmiXMLDoc.NewElement("View");
        adaphmiRowXMLElements->SetText(view.toLatin1().data());
        adaphmiInnerXMLElements->InsertEndChild(adaphmiRowXMLElements);
        adaphmiRowXMLElements = adaphmiXMLDoc.NewElement("Comments");
        adaphmiRowXMLElements->SetText(comments.toLatin1().data());
        adaphmiInnerXMLElements->InsertEndChild(adaphmiRowXMLElements);
        return adaphmiInnerXMLElements;
}

XMLElement* adaphmixmlcreator::generateLookUpTableElements(QString tagname,QString tagvalue)
{
        XMLElement* adaphmiInnerXMLElements = adaphmiXMLDoc.NewElement(tagname.toLatin1().data());
        adaphmiInnerXMLElements->SetText(tagvalue.toLatin1().data());
        return adaphmiInnerXMLElements;
}
QString adaphmixmlcreator::createLookTableDataforXML(QMap<adaphmisavelookuptable*,QString> looktableMap){
    QString loopTableData = "";
    QSet<int> rowData;
    QSet<int> colData;
    foreach( adaphmisavelookuptable* key, looktableMap.keys()){
        rowData.insert(key->getRow());
        colData.insert(key->getCol());
    }
    int row = rowData.count();
    int col = colData.count();
    adaphmisavelookuptable* gotTheKey = new adaphmisavelookuptable();
    for (int i = 0; i < row ; i++){
        loopTableData.append("\n\t\t");
        for (int j = 0; j < col; j++){
            foreach( adaphmisavelookuptable* key, looktableMap.keys()){
                if(key->getRow()==i && key->getCol() == j){
                    gotTheKey=key;
                }
            }
            loopTableData.append(looktableMap.value(gotTheKey));
            loopTableData.append("\t");
        }
    }
    loopTableData.append("\n\t");
    return loopTableData;
}
