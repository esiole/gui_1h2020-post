#include "datamodel.h"

DataModel::DataModel(QObject* parent) : QStringListModel(parent)
{

}

QStringList DataModel::print()
{
    return stringList();
}

void DataModel::addValue(const QString &value)
{
    QStringList data = stringList();
    if (data.indexOf(value) == -1)
    {
        data.append(value);
        setStringList(data);
        emit layoutChanged();
    }
}

void DataModel::deleteValue(const QString &value)
{
    QStringList data = stringList();
    if (!data.isEmpty())
    {
        int index = data.indexOf(value);
        if (index != -1)
        {
            data.removeAt(data.indexOf(value));
            setStringList(data);
            emit layoutChanged();
        }
    }
}
