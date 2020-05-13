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
#include <QDebug>
#include <QMessageBox>

#include "datamodel.h"
#include "windows.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent *event) override;

public slots:
    void doubleClickOnModelElement(const QModelIndex& index);
    void setMetaInfo();
    void durationChange(qint64 duration);
    void moveSlider();
    void deleteSong();
    void volumeChange(int volume);
    void openFiles();

private:
    Ui::MainWindow *ui;
    DataModel* dataModel;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    QTime* maxMediaDuration;
    QTime* currentMediaDuration;

    void calculationTime(QTime* time, qint64 millsec);
    void clearMetaInfo();
    void addMediaToModel(const QList<QUrl> &list);
};
#endif // MAINWINDOW_H
