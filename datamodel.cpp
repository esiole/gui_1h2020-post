#include "datamodel.h"

DataModel::DataModel(QObject* parent) : QStringListModel(parent)
{

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
    if (data.indexOf(value) == -1)
    {
        data.append(value);
        setStringList(data);
        emit layoutChanged();
    }
    return index;
}

/**
 * @brief DataModel::deleteValue
 * @param value Удаляемое значение
 */
void DataModel::deleteValue(const QString &value)
{
    QStringList data = stringList();
    if (data.indexOf(value) != -1)
    {
        data.removeAt(data.indexOf(value));
        setStringList(data);
        emit layoutChanged();
    }
}
