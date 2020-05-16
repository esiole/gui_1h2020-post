#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), columnCount(10)
{
    ui->setupUi(this);
    centralWidget()->setLayout(ui->horizontalLayout_5);
    this->resize(100, 100);
    setAcceptDrops(true);

    info = new MetaSongInfo();
    maxMediaDuration = new QTime(0, 0);
    currentMediaDuration = new QTime(0, 0);

    const int volume = 100;
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
    const int width = ui->graphicsView->width();
    const int height = ui->graphicsView->height();
    const int columnWidth = (width - 5 * (columnCount + 1)) / columnCount;
    scene->setSceneRect(0, 0, width, height);
    for (int i = 0, x = 5; i < columnCount; i++, x += columnWidth + 5)
    {
        Column* column = new Column(columnWidth, height);
        column->setPos(x, 0);
        connect(this, &MainWindow::startColumn, column, &Column::enableAnimation);
        connect(this, &MainWindow::stopColumn, column, &Column::disableAnimation);
        connect(player, &QMediaPlayer::currentMediaChanged, column, &Column::updateAnimation);
        scene->addItem(column);
    }

    connect(ui->tableView, &QTableView::doubleClicked, this, &MainWindow::doubleClickOnModelElement);
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
    connect(ui->hideAction, &QAction::triggered, this, &MainWindow::hideView);
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

void MainWindow::resizeEvent(QResizeEvent*)
{
    const int width = ui->graphicsView->width();
    const int height = ui->graphicsView->height();
    const int columnWidth = (width - 5 * (columnCount + 1)) / columnCount;
    scene->setSceneRect(0, 0, width, height);
    QList<QGraphicsItem*> list = scene->items();
    for (int i = 0, x = 5; i < columnCount; i++, x += columnWidth + 5)
    {
        Column* column = dynamic_cast<Column*>(list.at(i));
        column->setNewGeometry(columnWidth, height);
        column->setPos(x, 0);
    }
}

void MainWindow::doubleClickOnModelElement(const QModelIndex& index)
{
    dataModel->setCurrentMedia(index);
    player->play();
}

/**
 * @brief MainWindow::durationChange выводит прошедшее время с начала воспроизведения mediа, двигает slider
 * @param duration количество миллисекунд
 */
void MainWindow::durationChange(qint64 duration)
{
    calculationTime(currentMediaDuration, duration);
    const QString time = getStringFromTime(currentMediaDuration->minute(), currentMediaDuration->second());
    ui->labelCurrentDuration->setText(time);
    const int current = currentMediaDuration->msecsSinceStartOfDay();
    const int max = maxMediaDuration->msecsSinceStartOfDay();
    if (max != 0)
    {
        const int value = static_cast<int>(current / static_cast<double>(max) * 100);
        ui->sliderDuration->setValue(value);
    }
}

/**
 * @brief MainWindow::moveSlider пересчёт текущей позиции в медиа, исходя из положения слайдера
 */
void MainWindow::moveSlider()
{
    if (player->state() == QMediaPlayer::State::PlayingState)
    {
        const int value = ui->sliderDuration->sliderPosition();
        const int current = static_cast<int>(static_cast<double>(value) / 100 * maxMediaDuration->msecsSinceStartOfDay());
        player->setPosition(current);
    }
}

/**
 * @brief MainWindow::volumeChange изменяет уровень громкости и отображает его на виджете
 * @param volume значение громкости
 */
void MainWindow::volumeChange(int volume)
{
    player->setVolume(volume);
    ui->labelVolume->setText(QString::number(volume) + "%");
}

/**
 * @brief MainWindow::openFiles открытие окна с выбором mp3 файлов
 */
void MainWindow::openFiles()
{
    const QList<QUrl> list = QFileDialog::getOpenFileUrls(this, "Добавить mp3 файлы в плейлист", QUrl(), "*.mp3");
    addMediaToModel(list);
}

/**
 * @brief MainWindow::clearPlaylist удаляет все media
 */
void MainWindow::clearPlaylist()
{
    dataModel->clearData();
    clearMetaInfo();
    emit stopColumn();
}

/**
 * @brief MainWindow::deleteSong удаляет выделенную media
 */
void MainWindow::deleteSong()
{
    const int mediaCount = dataModel->deleteValue(ui->tableView->currentIndex());
    if (mediaCount == 0)
    {
        clearMetaInfo();
        emit stopColumn();
    }
    player->play();
}

/**
 * @brief MainWindow::setMetaInfo установка мета информации о текущей media
 */
void MainWindow::setMetaInfo()
{
    const QString author = player->metaData(QMediaMetaData::Author).toString();
    const QString title = player->metaData(QMediaMetaData::Title).toString();
    const QString album = player->metaData(QMediaMetaData::AlbumTitle).toString();
    const QString year = player->metaData(QMediaMetaData::Year).toString();
    emit setMeta(author, title, album, year);
    calculationTime(maxMediaDuration, player->metaData(QMediaMetaData::Duration).toLongLong());
    ui->labelMaxDuration->setText(getStringFromTime(maxMediaDuration->minute(), maxMediaDuration->second()));
    ui->labelFileName->setText(dataModel->getNameCurrentMedia());
}

/**
 * @brief MainWindow::playerStateChange остановка/запуск анимации столбиков
 * @param state состояние player
 */
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

/**
 * @brief MainWindow::hideView скрыть/показать представление, отображающее модель данных
 * нужна эмитация события resize для перерисовки столбиков на QGraphicsScene
 */
void MainWindow::hideView()
{
    if (ui->tableView->isVisible())
    {
        ui->tableView->hide();
        ui->hideAction->setText("Показать");
    }
    else
    {
        ui->tableView->show();
        ui->hideAction->setText("Скрыть");
    }
    this->resize(this->width() + 1, this->height() + 1);
    this->resize(this->width() - 1, this->height() - 1);
}

/**
 * @brief MainWindow::calculationTime расчёт времени исходя из количества миллисекунд
 * @param time в него устанавливается результат
 * @param millsec количество миллисекунд
 */
void MainWindow::calculationTime(QTime* time, qint64 millsec)
{
    int seconds = static_cast<int>(millsec / 1000);
    const int minutes = seconds / 60;
    seconds = seconds % 60;
    time->setHMS(0, minutes, seconds, millsec % 1000);
}

/**
 * @brief MainWindow::clearMetaInfo очистка мета информации о текущей media
 */
void MainWindow::clearMetaInfo()
{
    emit clearMeta();
    ui->labelMaxDuration->setText("");
    ui->labelCurrentDuration->setText("");
    ui->sliderDuration->setValue(0);
}

/**
 * @brief MainWindow::addMediaToModel добавляет media в модель данных
 * @param list список url media
 */
void MainWindow::addMediaToModel(const QList<QUrl> &list)
{
    foreach (QUrl elem, list)
    {
        if (QFileInfo(elem.toLocalFile()).suffix() == "mp3")
        {
            dataModel->addValue(elem);
        }
    }
}

/**
 * @brief MainWindow::getStringFromTime формирует строку времени в шаблоне "XX:XX"
 * @param minutes количество минут
 * @param seconds количество секунд
 * @return строка времени, если количество минут < 10, то первая цифра опускается
 */
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
