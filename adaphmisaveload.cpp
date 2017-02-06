#include "adaphmisaveload.h"
#include "adaphmisavelookuptable.h"
#include <QSettings>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
adaphmisaveload::adaphmisaveload(QString m_adapSettingsFile){
    this->m_adapSettingsFile = m_adapSettingsFile;
}
void adaphmisaveload::saveSettings(const QString &key, const QVariant &value, const QString &group)
{
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();

}
QVariant adaphmisaveload::loadSettings(const QString &key, const QString &group, const QVariant &defaultValue = QVariant())
{
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    QVariant value = settings.value(key, defaultValue);
    settings.endGroup();
    return value;
}
void adaphmisaveload::saveSettingsMap(QMap<QString,QString> lutInputOutputMap,const QString &group)
{
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    QMap<QString, QString>::const_iterator i = lutInputOutputMap.constBegin();
    while (i != lutInputOutputMap.constEnd()) {
        settings.setValue(i.key(), i.value());
        ++i;
    }
    settings.endGroup();
}
QMap<QString, QString> adaphmisaveload::loadSettingsMap(const QString &group)
{
    QMap<QString, QString> lutInputOutputMap;
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    QStringList keys = settings.childKeys();
    foreach (QString key, keys) {
         lutInputOutputMap[key] = settings.value(key).toString();
    }
    settings.endGroup();
    return lutInputOutputMap;
}
void adaphmisaveload::savelookupTableMap(QMap<adaphmisavelookuptable*,QString> lutInputOutputMap,const QString &group)
{
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup(group);
    QMap<adaphmisavelookuptable*, QString>::const_iterator i = lutInputOutputMap.constBegin();
    while (i != lutInputOutputMap.constEnd()) {
        settings.setValue(i.key()->createString(), i.value());
        ++i;
    }
    settings.endGroup();
}
QMap<adaphmisavelookuptable*, QString> adaphmisaveload::loadlookupTableMap(const QString &group)
{
    QMap<adaphmisavelookuptable*, QString> lutInputOutputMap;
    QSettings settings(this->m_adapSettingsFile, QSettings::IniFormat);
    settings.beginGroup(group);
    QStringList keys = settings.childKeys();
    foreach (QString key, keys) {
        adaphmisavelookuptable *rowcolString = new adaphmisavelookuptable();
        rowcolString->breakString(key);
        lutInputOutputMap[rowcolString] = settings.value(key).toString();
    }
    settings.endGroup();
    return lutInputOutputMap;
}
