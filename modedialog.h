#ifndef MODEDIALOG_H
#define MODEDIALOG_H

#include <QDialog>

#include "header.h"

namespace Ui {
class ModeDialog;
}

class ModeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModeDialog(QWidget *parent = 0);
    ~ModeDialog();

public slots:
    void client();
    void server();

private:
    Ui::ModeDialog *ui;
};

#endif // MODEDIALOG_H
