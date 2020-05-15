#include "metasonginfo.h"
#include "ui_metasonginfo.h"

MetaSongInfo::MetaSongInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetaSongInfo)
{
    ui->setupUi(this);
}

MetaSongInfo::~MetaSongInfo()
{
    delete ui;
}

/**
 * @brief MetaSongInfo::setInfo устанавливает мета информацию о файле на форму
 * @param author исполнитель
 * @param title название
 * @param album альбом
 * @param year год выхода
 */
void MetaSongInfo::setInfo(const QString &author, const QString &title, const QString &album, const QString &year)
{
    ui->labelPlayer->setText(author);
    ui->labelTitle->setText(title);
    ui->labelAlbum->setText(album);
    ui->labelYear->setText(year);
}

/**
 * @brief MetaSongInfo::clearInfo удаляет всю информацию из формы
 */
void MetaSongInfo::clearInfo()
{
    ui->labelPlayer->setText("");
    ui->labelTitle->setText("");
    ui->labelAlbum->setText("");
    ui->labelYear->setText("");
}
