#include "ipclient.h"
#include "ui_ipclient.h"

#include <cstdio>
#include <QPushButton>
#include <QDebug>

IpClient::IpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpClient)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() &~ (
                   Qt::WindowCloseButtonHint |
                   Qt::WindowMaximizeButtonHint |
                   Qt::WindowMinimizeButtonHint));

    ui->lineEdit->setText("127.0.0.1");

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(checkIp()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    mapper = new QSignalMapper(this);
    grid = new QGridLayout();
    char tmp[100];

    for (int i = 0; i < 10; i++)
    {
        sprintf(tmp, "%d", i + 1);
        addButton(tmp, i + 1, i / 3, i % 3);
    }
    addButton("0", 0, 3, 0);
    addButton(".", MAP_DOT, 3, 1);
    addButton("<-", MAP_BAK, 3, 2);

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(clicked(int)));
    ui->horizontalLayout->insertLayout(0, grid, 1);
}

IpClient::~IpClient()
{
    delete mapper;
    delete ui;
}

void IpClient::addButton(const char *text, int index, int r, int c)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, index);
    grid->addWidget(button, r, c);
}

void IpClient::clicked(int k)
{
    QString now(ui->lineEdit->text());

    if (k == MAP_BAK)
    {
        if (now.length() > 0)
        {
            now.remove(now.length() - 1, 1);
        }
    }
    else
    {
        if (k == MAP_DOT)
        {
            now.append(".");
        }
        else
        {
            now.append(k + 48);
        }
    }
    ui->lineEdit->setText(now);
}

void IpClient::cancel()
{
    done(0);
}

void IpClient::checkIp()
{
    QString now = ui->lineEdit->text();

    for (int i = 0; i < now.length(); i++)
    {
        if (now[i] != '.' && !(now[i] >= '0' && now[i] <= '9'))
        {
            ui->lineEdit->setText("");
            ui->label->setText("Invalid IP address. Try again");
            return;
        }
    }
    QStringList subs = now.split(".");
    if (subs.size() != 4)
    {
        ui->lineEdit->setText("");
        ui->label->setText("Invalid IP address. Try again");
        return;
    }
    for (int i = 0; i < subs.size(); i++)
    {
        QString sub = subs[i];
        bool flag;
        int t = sub.toInt(&flag);
        if (!flag || !(t >= 0 && t < 256))
        {
            ui->lineEdit->setText("");
            ui->label->setText("Invalid IP address. Try again");
            return;
        }
    }
    done(1);
}

QString IpClient::getIp() const
{
    qDebug() << ui->lineEdit->text();
    return ui->lineEdit->text();
}
