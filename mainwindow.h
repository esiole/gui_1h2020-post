#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QMediaMetaData>
#include <QTime>
#include <QFileDialog>

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
    virtual void dropEvent(QDropEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private slots:
    void doubleClickOnModelElement(const QModelIndex& index);
    void durationChange(qint64 duration);
    void moveSlider();
    void volumeChange(int volume);
    void openFiles();
    void clearPlaylist();
    void deleteSong();
    void setMetaInfo();
    void playerStateChange(QMediaPlayer::State state);
    void hideView();

signals:
    void stopColumn();
    void startColumn();
    void setMeta(const QString &author, const QString &title, const QString &album, const QString &year);
    void clearMeta();

private:
    Ui::MainWindow *ui;
    DataModel* dataModel;
    QMediaPlayer* player;
    QTime* maxMediaDuration;
    QTime* currentMediaDuration;
    QGraphicsScene* scene;
    MetaSongInfo* info;
    const int columnCount;

    void calculationTime(QTime* time, qint64 millsec);
    void clearMetaInfo();
    void addMediaToModel(const QList<QUrl> &list);
    QString getStringFromTime(int minutes, int seconds) const;
};
#endif // MAINWINDOW_H
