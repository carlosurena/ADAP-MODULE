#ifndef ADAPHMISAVELOAD_H
#define ADAPHMISAVELOAD_H
#include <QMainWindow>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include "adaphmisavelookuptable.h"

class adaphmisaveload
{
public:
    adaphmisaveload(QString m_adapSettingsFile);
    void saveSettings (const QString &key, const QVariant &value, const QString &group);
    QVariant loadSettings(const QString &key, const QString &group, const QVariant &defaultValue);
    void saveSettingsMap(QMap<QString, QString> lutInputOutputMap, const QString &group);
    QMap<QString,QString> loadSettingsMap(const QString &group);
    QMap<adaphmisavelookuptable*, QString> loadlookupTableMap(const QString &group);
    void savelookupTableMap(QMap<adaphmisavelookuptable*,QString> lutInputOutputMap,const QString &group);
private:
    QString m_adapSettingsFile;
};

#endif // ADAPHMISAVELOAD_H
