#include "extimer.h"

#include <QTimer>
#include <QTime>

ExTimer::ExTimer(int incr, QLabel *board, QObject *parent) :
    QObject(parent),
    timeBoard(board),
    incrMode(incr), inCounting(0)
{
    secTimer = new QTimer(this);

    connect(secTimer, SIGNAL(timeout()), this, SLOT(secPass()));
}

void ExTimer::restart(int sec)
{
    if (!sec)
    {
        inCounting = 0;
        timeBoard->setText("00:00");
        return;
    }
    totalSec = restSec = sec;
    secTimer->start(1000);
    inCounting = 1;
}

void ExTimer::secPass()
{
    if (!inCounting) return;
    restSec--;
    if (restSec)
    {
        secTimer->start(1000);
        QTime time;
        int mm = 0, ss;
        if (incrMode)
            ss = totalSec - restSec;
        else
            ss = restSec;
        mm += ss / 60;
        ss %= 60;
        mm %= 60;
        time.setHMS(0, mm, ss);
        timeBoard->setText(time.toString("mm:ss"));
    }
    else
    {
        inCounting = 0;
        timeBoard->setText("00:00");
        emit bomb();
    }
}

void ExTimer::resume()
{
    if (!restSec) return;
    inCounting = 1;
    secTimer->start(1000);
}

void ExTimer::pause()
{
    inCounting = 0;
}
