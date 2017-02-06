#ifndef ADAPHMISAVELOOKUPTABLE_H
#define ADAPHMISAVELOOKUPTABLE_H
#include <QModelIndex>

class adaphmisavelookuptable
{
public:
    adaphmisavelookuptable();
    ~adaphmisavelookuptable();
    adaphmisavelookuptable(int row, int col);
    int getRow();
    int getCol();
    void setRow(int row);
    void setCol(int col);
    QString createString();
    void breakString(QString rowcol);
private:
    int row;
    int col;
};

#endif // ADAPHMISAVELOOKUPTABLE_H
