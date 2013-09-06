#ifndef IPSERVER_H
#define IPSERVER_H

#include <QDialog>

namespace Ui {
class IpServer;
}

class IpServer : public QDialog
{
    Q_OBJECT

public:
    explicit IpServer(QWidget *parent = 0);
    ~IpServer();

    QString getIp() const;

public slots:
    void confirmed();
    void cancel();

private:
    Ui::IpServer *ui;
};

#endif // IPSERVER_H
