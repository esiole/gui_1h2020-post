#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAcceptDrops(true);

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
                if (player->isMetaDataAvailable())
                    QMessageBox::information(this, "", player->metaData(QMediaMetaData::Title).toString());
            }
        }
    }
}

void MainWindow::doubleClickOnModelElement(const QModelIndex& index)
{
    playlist->setCurrentIndex(index.row());
}
