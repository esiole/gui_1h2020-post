#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QStringListModel>

class DataModel : public QStringListModel
{
    Q_OBJECT

public:
    DataModel(QObject* parent);
    QString getValue(int row) const;

public slots:
    int addValue(const QString &value);
    int deleteValue(const QModelIndex &index);
    void clearData();
};

#endif // DATAMODEL_H
