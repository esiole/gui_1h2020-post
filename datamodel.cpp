#include "datamodel.h"

DataModel::DataModel(QObject* parent) : QStringListModel(parent)
{

}

QStringList DataModel::print()
{
    return stringList();
}

/**
 * @brief DataModel::addValue
 * @param value Добавляемое значение
 * @return -1, если такого элемента нет в модели, иначе какое-то число
 */
int DataModel::addValue(const QString &value)
{
    QStringList data = stringList();
    int index = data.indexOf(value);
    if (index == -1)
    {
        data.append(value);
        setStringList(data);
        emit layoutChanged();
    }
    return index;
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
