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
 * @param index Индекс удаляемого элемента
 * @return Позиция удаляемого элемента в списке
 */
int DataModel::deleteValue(const QModelIndex &index)
{
    QStringList data = stringList();
    int pos = index.row();
    data.removeAt(pos);
    setStringList(data);
    emit layoutChanged();
    return pos;
}

/**
 * @brief DataModel::clearData
 */
void DataModel::clearData()
{
    QStringList data = stringList();
    data.clear();
    setStringList(data);
    emit layoutChanged();
}

QString DataModel::getValue(int row)
{
    QStringList data = stringList();
    QString value = "";
    if (row >= 0 && row < data.size())
    {
        value = data.at(row);
    }
    return value;
}
