#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QListView>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTime>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QDebug>

#include "datamodel.h"
#include "column.h"
#include "metasonginfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent *event) override;

public slots:
    void doubleClickOnModelElement(const QModelIndex& index);
    //void setMetaInfo();
    void durationChange(qint64 duration);
    void moveSlider();
    void deleteSong();
    void volumeChange(int volume);
    void openFiles();
    void clearPlaylist();
    void playerStateChange(QMediaPlayer::State state);
    void setMetaInfo();

signals:
    void stopColumn();
    void startColumn();
    void setMeta(const QString &author, const QString &title, const QString &album, const QString &year);
    void clearMeta();

private:
    Ui::MainWindow *ui;
    DataModel* dataModel;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    QTime* maxMediaDuration;
    QTime* currentMediaDuration;
    QGraphicsScene* scene;
    MetaSongInfo* info;

    void calculationTime(QTime* time, qint64 millsec);
    void clearMetaInfo();
    void addMediaToModel(const QList<QUrl> &list);
};
#endif // MAINWINDOW_H
