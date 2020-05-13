#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

class DataModel : public QStringListModel
{
    Q_OBJECT

private:

public:
    DataModel(QObject* parent);
    QStringList print();

public slots:
    void addValue(const QString &value);
    void deleteValue(const QString &value);
};

#endif // DATAMODEL_H
