#include "adaphmisavelookuptable.h"
#include <QStringList>
#include <QObject>
adaphmisavelookuptable::adaphmisavelookuptable(){

}
adaphmisavelookuptable::~adaphmisavelookuptable(){

}
adaphmisavelookuptable::adaphmisavelookuptable(int row,int col){
    this->row = row;
    this->col = col;    
}
int adaphmisavelookuptable::getRow(){
    return this->row;
}
int adaphmisavelookuptable::getCol(){
    return this->col;
}
void adaphmisavelookuptable::setRow(int row){
    this->row = row;
}
void adaphmisavelookuptable::setCol(int col){
    this->col = col;
}
QString adaphmisavelookuptable::createString(){
    QString qSrow = QString::number(this->getRow());
    QString qScol = QString::number(this->getCol());
    QString returnVal = qSrow+"_"+qScol;
    return returnVal;
}
void adaphmisavelookuptable::breakString(QString rowcol){
    QStringList qrowcol = rowcol.split("_");
    this->row=qrowcol[0].toInt();
    this->col=qrowcol[1].toInt();
}
