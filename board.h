#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QLabel>

#include "header.h"
#include "OAlgo.h"

class MainWindow;

class CellLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CellLabel(Board *bd, QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Board *father;
};

class Board : public QWidget
{
    Q_OBJECT
    friend class OAlgo;
    friend class CellLabel;

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
    int needHint, needPlay;
    int mouseEnable;
    int noPlace;
    int currentPaintLevel;
    int userColor;
    int userColorLocal;
    int TOTALSIZE, BARGIN, PIECEBARGIN;
    int CELLSIZE, GAPSIZE, PIECEOFFSET, x0, y0;
    CellState bdState[64];

    CellLabel *cellArray[64];

    void initResources();
    void initLabels();

    void paintPieces(int hint, int play);

    QColor penColor, bkgColor, cellColorA, cellColorB, pieceColorW, pieceColorB;

    void trySetPiece(int r, int c);
    void hintPieces();

    void gameEnd(int msg);

    QPoint mousePressPos;
    QPoint mousePrevCell, mouseCell;

    QPoint getMouseCell(QPoint pos);

    OAlgo *algo;
    QImage uiWindow, uiPieceB, uiPieceW;
};

#endif // BOARD_H
