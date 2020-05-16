#include "datamodel.h"

DataModel::DataModel(QMediaPlayer* player, QObject* parent) : QAbstractListModel(parent)
{
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
}

int DataModel::rowCount(const QModelIndex &) const
{
    return playlist->mediaCount();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return getNameMedia(index.row());
    }
    return QVariant();
}

/**
 * @brief DataModel::setCurrentMedia устанавливает текущее медиа в плейлисте
 * @param index индекс устанавливаемой медиа
 */
void DataModel::setCurrentMedia(const QModelIndex &index)
{
    playlist->setCurrentIndex(index.row());
}

/**
 * @brief DataModel::getNameCurrentMedia
 * @return имя файла текущей медиа в плейлисте
 */
QString DataModel::getNameCurrentMedia() const
{
    return getNameMedia(playlist->currentIndex());
}

/**
 * @brief DataModel::getNameMedia
 * @param pos позиция медиа в playlist
 * @return название файла медиа
 */
QString DataModel::getNameMedia(int pos) const
{
    QString path = playlist->media(pos).request().url().toLocalFile();
    return QFileInfo(path).completeBaseName();
}

/**
 * @brief DataModel::isExistMedia проверяет, находится ли медиа в playlist
 * @param url адрес медиа
 * @return true если присутствует, при отсутствии false
 */
bool DataModel::isExistMedia(const QUrl &url) const
{
    bool isExist = false;
    for (int i = 0; i < playlist->mediaCount(); i++)
    {
        if (playlist->media(i) == url)
        {
            isExist = true;
            break;
        }
    }
    return isExist;
}

/**
 * @brief DataModel::addValue добавляет элемент в модель
 * @param value добавляемое значение
 * @return -1, если такого элемента нет в модели, иначе какое-то число
 */
void DataModel::addValue(const QUrl &path)
{
    if (!isExistMedia(path))
    {
        playlist->addMedia(path);
        emit layoutChanged();
    }
}

/**
 * @brief DataModel::deleteValue удаляет элемент из модели
 * @param index индекс удаляемого элемента
 * @return количество оставшихся элементов в модели
 */
int DataModel::deleteValue(const QModelIndex &index)
{
    /**
     * при удалении первого элемента в плейлисте
     * воспроизведение останавливается;
     * чтобы вернуться к игравшей до этого песне
     */
    const int pos = index.row();
    if (pos == 0)
    {
        const int currentSong = playlist->currentIndex();
        playlist->removeMedia(pos);
        playlist->setCurrentIndex(currentSong - 1);
    }
    else
    {
        playlist->removeMedia(pos);
    }
    emit layoutChanged();
    return playlist->mediaCount();
}

/**
 * @brief DataModel::clearData удаляет все элементы из модели
 */
void DataModel::clearData()
{
    playlist->clear();
    emit layoutChanged();
}

void DataModel::next()
{
    playlist->next();
}

void DataModel::previous()
{
    playlist->previous();
}
