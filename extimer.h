#ifndef EXTIMER_H
#define EXTIMER_H

#include <QTimer>
#include <QLabel>

class ExTimer : public QObject
{
    Q_OBJECT
public:
    explicit ExTimer(int incr = 0, QLabel *board = 0, QObject *parent = 0);
    void restart(int sec);

signals:
    void bomb();

public slots:
    void resume();
    void pause();
    void secPass();

private:
    QTimer *secTimer;
    QLabel *timeBoard;
    int restSec, totalSec;
    int incrMode, inCounting;
};

#endif // EXTIMER_H
