#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    maxMediaDuration = new QTime(0, 0);
    currentMediaDuration = new QTime(0, 0);
    dataModel = new DataModel(this);

    ui->listView->setModel(dataModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    player->setVolume(100);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    connect(ui->listView, &QListView::doubleClicked, this, &MainWindow::doubleClickOnModelElement);

    connect(ui->playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
    connect(ui->nextButton, &QPushButton::clicked, playlist, &QMediaPlaylist::next);
    connect(ui->prevButton, &QPushButton::clicked, playlist, &QMediaPlaylist::previous);
    connect(ui->pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);

    connect(player, QOverload<>::of(&QMediaObject::metaDataChanged), this, &MainWindow::setMetaInfo);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::durationChange);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        for (int i = 0; i < urlList.size(); i++)
        {
            QString path = urlList.at(i).toLocalFile();
            QFileInfo* info = new QFileInfo(path);
            QString suffix = QFileInfo(path).suffix();

            if (suffix == "mp3")
            {
                int index = dataModel->addValue(info->baseName());
                if (index == -1)
                {
                    playlist->addMedia(urlList.at(i));
                }
            }
        }
    }
}

void MainWindow::doubleClickOnModelElement(const QModelIndex& index)
{
    playlist->setCurrentIndex(index.row());
    player->play();
}

void MainWindow::setMetaInfo()
{
    ui->labelPlayer->setText(player->metaData(QMediaMetaData::Author).toString());
    ui->labelTitle->setText(player->metaData(QMediaMetaData::Title).toString());
    ui->labelAlbum->setText(player->metaData(QMediaMetaData::AlbumTitle).toString());
    ui->labelYear->setText(player->metaData(QMediaMetaData::Year).toString());
    calculationTime(maxMediaDuration, player->metaData(QMediaMetaData::Duration).toLongLong());
    QString time = QString::number(maxMediaDuration->minute()) + ":" + QString::number(maxMediaDuration->second());
    ui->labelMaxDuration->setText(time);
}

void MainWindow::durationChange(qint64 duration)
{
    calculationTime(currentMediaDuration, duration);
    QString time = "";
    int second = currentMediaDuration->second();
    int minute = currentMediaDuration->minute();
    if (second < 10)
    {
        if (second == 0)
        {
            time = QString::number(currentMediaDuration->minute()) + ":00";
        }
        time = QString::number(minute) + ":0" + QString::number(second);
    }
    else
    {
        time = QString::number(minute) + ":" + QString::number(second);
    }
    ui->labelCurrentDuration->setText(time);
    int current = currentMediaDuration->msecsSinceStartOfDay();
    int max = maxMediaDuration->msecsSinceStartOfDay();
    if (max != 0)
    {
        int value = static_cast<int>(current / static_cast<double>(max) * 100);
        ui->sliderDuration->setValue(value);
    }
}

void MainWindow::calculationTime(QTime* time, qint64 millsec)
{
    int seconds = static_cast<int>(millsec / 1000);
    int minutes = seconds / 60;
    seconds = seconds % 60;
    time->setHMS(0, minutes, seconds, millsec % 1000);
}
