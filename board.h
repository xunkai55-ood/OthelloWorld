#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "header.h"
#include "oalgo.h"

class MainWindow;

class Board : public QWidget
{
    Q_OBJECT
    friend class oAlgo;

public:
    explicit Board(int userC, QWidget *parent = 0);
    ~Board();

signals:
    void decide(int r, int c);

public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int mouseEnable;
    int noPlace;
    int currentPaintLevel;
    int userColor;
    int userColorLocal;
    int CELLSIZE, GAPSIZE, TOTALSIZE, BARGIN, PIECEBARGIN, x0, y0;
    CellState bdState[64];

    QColor penColor, bkgColor, cellColorA, cellColorB, pieceColorW, pieceColorB;

    void trySetPiece(int r, int c);

    void gameEnd(int msg);

    QPoint mousePressPos;
    QPoint mousePrevCell, mouseCell;

    QPoint getMouseCell(QPoint pos);

    oAlgo *algo;
};

#endif // BOARD_H
