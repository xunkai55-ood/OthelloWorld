#include "choosecolor.h"

#include <QImage>
#include <QIcon>
#include <QHBoxLayout>
#include <QPushButton>

ChooseColor::ChooseColor(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() &~ (
                   Qt::WindowCloseButtonHint |
                   Qt::WindowMaximizeButtonHint |
                   Qt::WindowMinimizeButtonHint));
    resize(250, 150);

    blackButton = initButton("://ui/heiqi.tga");
    whiteButton = initButton("://ui/baiqi.tga");

    blackButton->setGeometry(50, 50, 50, 50);
    whiteButton->setGeometry(150, 50, 50, 50);

    connect(blackButton, SIGNAL(clicked()), this, SLOT(chooseB()));
    connect(whiteButton, SIGNAL(clicked()), this, SLOT(chooseW()));
}

QPushButton *ChooseColor::initButton(const char *icoFile)
{
    QPixmap icoPm(icoFile);
    QIcon ico(icoPm);
    QPushButton *button = new QPushButton(ico, "", this);
    //button->setFlat(true);
    button->setIcon(ico);
    button->setIconSize(icoPm.size());
    return button;
}

void ChooseColor::chooseB()
{
    done(BLACK);
}

void ChooseColor::chooseW()
{
    done(WHITE);
}
