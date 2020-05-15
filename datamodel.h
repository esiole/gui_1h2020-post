#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileInfo>

class DataModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DataModel(QMediaPlayer* player, QObject* parent);
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    void setCurrentMedia(const QModelIndex &index);
    QString getNameCurrentMedia() const;

private:
    QMediaPlaylist* playlist;

    QString getNameMedia(int pos) const;
    bool isExistMedia(const QUrl& url) const;

public slots:
    void addValue(const QUrl &path);
    int deleteValue(const QModelIndex &index);
    void clearData();
    void next();
    void previous();
};

#endif // DATAMODEL_H
