#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    info = new MetaSongInfo();
    maxMediaDuration = new QTime(0, 0);
    currentMediaDuration = new QTime(0, 0);

    int volume = 100;
    ui->sliderDuration->setValue(0);
    ui->labelVolume->setText(QString::number(volume) + "%");
    ui->sliderVolume->setValue(volume);
    ui->sliderVolume->setTickPosition(QSlider::TickPosition::TicksRight);

    player = new QMediaPlayer(this);
    player->setVolume(volume);
    dataModel = new DataModel(player, this);
    ui->tableView->setModel(dataModel);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    int columnCount = 10;
    int columnWidth = (width - 5 * (columnCount + 1)) / columnCount;
    scene->setSceneRect(0, 0, width, height);
    for (int i = 0, x = 5; i < columnCount; i++, x += columnWidth + 5)
    {
        Column* column = new Column(100, columnWidth);
        column->setPos(x, 0);
        connect(this, &MainWindow::startColumn, column, &Column::enableAnimation);
        connect(this, &MainWindow::stopColumn, column, &Column::disableAnimation);
        connect(player, &QMediaPlayer::currentMediaChanged, column, &Column::updateAnimation);
        scene->addItem(column);
    }

    connect(ui->tableView, &QListView::doubleClicked, this, &MainWindow::doubleClickOnModelElement);
    connect(ui->playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
    connect(ui->nextButton, &QPushButton::clicked, dataModel, &DataModel::next);
    connect(ui->prevButton, &QPushButton::clicked, dataModel, &DataModel::previous);
    connect(ui->pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
    connect(ui->sliderDuration, &QSlider::sliderReleased, this, &MainWindow::moveSlider);   
    connect(ui->sliderVolume, &QSlider::valueChanged, this, &MainWindow::volumeChange);
    connect(ui->openFilesAction, &QAction::triggered, this, &MainWindow::openFiles);
    connect(ui->clearAction, &QAction::triggered, this, &MainWindow::clearPlaylist); 
    connect(ui->delAction, &QAction::triggered, this, &MainWindow::deleteSong);
    connect(ui->metaAction, &QAction::triggered, info, &MetaSongInfo::show);
    connect(player, QOverload<>::of(&QMediaObject::metaDataChanged), this, &MainWindow::setMetaInfo);
    connect(this, &MainWindow::setMeta, info, &MetaSongInfo::setInfo);
    connect(this, &MainWindow::clearMeta, info, &MetaSongInfo::clearInfo);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::durationChange);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::playerStateChange);
    connect(this, &MainWindow::destroyed, info, &MetaSongInfo::close);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete maxMediaDuration;
    delete currentMediaDuration;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        addMediaToModel(mimeData->urls());
    }
}

void MainWindow::closeEvent(QCloseEvent*)
{
    info->close();
}

void MainWindow::doubleClickOnModelElement(const QModelIndex& index)
{
    dataModel->setCurrentMedia(index);
    player->play();
}

void MainWindow::durationChange(qint64 duration)
{
    calculationTime(currentMediaDuration, duration);
    QString time = getStringFromTime(currentMediaDuration->minute(), currentMediaDuration->second());
    ui->labelCurrentDuration->setText(time);
    int current = currentMediaDuration->msecsSinceStartOfDay();
    int max = maxMediaDuration->msecsSinceStartOfDay();
    if (max != 0)
    {
        int value = static_cast<int>(current / static_cast<double>(max) * 100);
        ui->sliderDuration->setValue(value);
    }
}

void MainWindow::moveSlider()
{
    if (player->state() == QMediaPlayer::State::PlayingState)
    {
        int value = ui->sliderDuration->sliderPosition();
        int current = static_cast<int>(static_cast<double>(value) / 100 * maxMediaDuration->msecsSinceStartOfDay());
        player->setPosition(current);
    }
}

void MainWindow::volumeChange(int volume)
{
    player->setVolume(volume);
    ui->labelVolume->setText(QString::number(volume) + "%");
}

void MainWindow::openFiles()
{
    QList<QUrl> list = QFileDialog::getOpenFileUrls(this, "Добавить mp3 файлы в плейлист", QUrl(), "*.mp3");
    addMediaToModel(list);
}

void MainWindow::clearPlaylist()
{
    dataModel->clearData();
    clearMetaInfo();
    emit stopColumn();
}

void MainWindow::deleteSong()
{
    QModelIndex index  = ui->tableView->currentIndex();
    int mediaCount = dataModel->deleteValue(index);
    if (mediaCount == 0)
    {
        clearMetaInfo();
        emit stopColumn();
    }
    player->play();
}

void MainWindow::setMetaInfo()
{
    QString author = player->metaData(QMediaMetaData::Author).toString();
    QString title = player->metaData(QMediaMetaData::Title).toString();
    QString album = player->metaData(QMediaMetaData::AlbumTitle).toString();
    QString year = player->metaData(QMediaMetaData::Year).toString();
    emit setMeta(author, title, album, year);
    calculationTime(maxMediaDuration, player->metaData(QMediaMetaData::Duration).toLongLong());
    ui->labelMaxDuration->setText(getStringFromTime(maxMediaDuration->minute(), maxMediaDuration->second()));
    ui->labelFileName->setText(dataModel->getNameCurrentMedia());
}

void MainWindow::playerStateChange(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::State::PlayingState)
    {
        emit startColumn();
    }
    if (state == QMediaPlayer::State::PausedState)
    {
        emit stopColumn();
    }
}

void MainWindow::calculationTime(QTime* time, qint64 millsec)
{
    int seconds = static_cast<int>(millsec / 1000);
    int minutes = seconds / 60;
    seconds = seconds % 60;
    time->setHMS(0, minutes, seconds, millsec % 1000);
}

void MainWindow::clearMetaInfo()
{
    emit clearMeta();
    ui->labelMaxDuration->setText("");
    ui->labelCurrentDuration->setText("");
    ui->sliderDuration->setValue(0);
}

void MainWindow::addMediaToModel(const QList<QUrl> &list)
{
    for (int i = 0; i < list.size(); i++)
    {
        QString path = list.at(i).toLocalFile();
        if (QFileInfo(path).suffix() == "mp3")
        {
            dataModel->addValue(list.at(i));
        }
    }
}

QString MainWindow::getStringFromTime(int minutes, int seconds) const
{
    QString time = "";
    if (seconds < 10)
    {
        if (seconds == 0)
        {
            time = QString::number(currentMediaDuration->minute()) + ":00";
        }
        time = QString::number(minutes) + ":0" + QString::number(seconds);
    }
    else
    {
        time = QString::number(minutes) + ":" + QString::number(seconds);
    }
    return time;
}
