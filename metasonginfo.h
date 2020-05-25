#ifndef METASONGINFO_H
#define METASONGINFO_H

#include <QWidget>

namespace Ui {
class MetaSongInfo;
}

class MetaSongInfo : public QWidget
{
    Q_OBJECT

public:
    explicit MetaSongInfo(QWidget *parent = nullptr);
    ~MetaSongInfo();

public slots:
    void setInfo(const QString &author, const QString &title, const QString &album, const QString &year);
    void clearInfo();

private:
    Ui::MetaSongInfo *ui;
};

#endif // METASONGINFO_H
