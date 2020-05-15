#include "datamodel.h"

DataModel::DataModel(QObject* parent) : QStringListModel(parent)
{

}

/**
 * @brief DataModel::addValue добавляет элемент в модель
 * @param value добавляемое значение
 * @return -1, если такого элемента нет в модели, иначе какое-то число
 */
int DataModel::addValue(const QString &value)
{
    QStringList data = stringList();
    const int index = data.indexOf(value);
    if (index == -1)
    {
        data.append(value);
        setStringList(data);
        emit layoutChanged();
    }
    return index;
}

/**
 * @brief DataModel::deleteValue удаляет элемент из модели
 * @param index индекс удаляемого элемента
 * @return позиция удаляемого элемента в списке
 */
int DataModel::deleteValue(const QModelIndex &index)
{
    QStringList data = stringList();
    const int pos = index.row();
    data.removeAt(pos);
    setStringList(data);
    emit layoutChanged();
    return pos;
}

/**
 * @brief DataModel::clearData удаляет все элементы из модели
 */
void DataModel::clearData()
{
    QStringList data = stringList();
    data.clear();
    setStringList(data);
    emit layoutChanged();
}

/**
 * @brief DataModel::getValue возвращает значение элемента из модели
 * @param row позиция элемента в модели
 * @return значение элемента
 */
QString DataModel::getValue(int row) const
{
    const QStringList data = stringList();
    QString value = "";
    if (row >= 0 && row < data.size())
    {
        value = data.at(row);
    }
    return value;
}
