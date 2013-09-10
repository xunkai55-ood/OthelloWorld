#include "modedialog.h"
#include "ui_modedialog.h"

ModeDialog::ModeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModeDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() &~ (
                   Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint |
                   Qt::WindowMaximizeButtonHint |
                   Qt::WindowMinimizeButtonHint));

    connect(ui->clientButton, SIGNAL(clicked()), this, SLOT(client()));
    connect(ui->serverButton, SIGNAL(clicked()), this, SLOT(server()));
}

ModeDialog::~ModeDialog()
{
    delete ui;
}

void ModeDialog::client()
{
    done(MODE_CLIENT);
}

void ModeDialog::server()
{
    done(MODE_SERVER);
}
