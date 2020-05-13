#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

class DataModel : public QStringListModel
{
    Q_OBJECT

public:
    DataModel(QObject* parent);

public slots:
    int addValue(const QString &value);
    void deleteValue(const QString &value);
};

#endif // DATAMODEL_H
