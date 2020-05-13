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

private:
    Ui::MainWindow *ui;
    DataModel* dataModel;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
};
#endif // MAINWINDOW_H