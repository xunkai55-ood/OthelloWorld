#ifndef IPCLIENT_H
#define IPCLIENT_H

#include <QDialog>
#include <QSignalMapper>
#include <QGridLayout>

#define MAP_DOT 11
#define MAP_BAK 12

namespace Ui {
class IpClient;
}

class IpClient : public QDialog
{
    Q_OBJECT

public:
    explicit IpClient(QWidget *parent = 0);
    ~IpClient();

    QString getIp() const;

public slots:
    void checkIp();
    void cancel();

    void clicked(int k);

private:
    Ui::IpClient *ui;
    QSignalMapper *mapper;
    QGridLayout *grid;

    void addButton(const char *text, int index, int r, int c);
};

#endif // IPCLIENT_H
