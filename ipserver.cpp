#include "ipserver.h"
#include "ui_ipserver.h"

IpServer::IpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpServer)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(confirmed()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setText(getIp());
}

IpServer::~IpServer()
{
    delete ui;
}

void IpServer::confirmed()
{
    done(1);
}

void IpServer::cancel()
{
    done(0);
}

QString IpServer::getIp() const
{
    return QString("127.0.0.1");
}
