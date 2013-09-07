#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include <QDialog>
#include "header.h"

class ChooseColor : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseColor(QWidget *parent = 0);

signals:

public slots:
    void chooseB();
    void chooseW();

private:
    QPushButton *blackButton, *whiteButton;
    QPushButton *initButton(const char *icoFile);
};

#endif // CHOOSECOLOR_H
