#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QLabel>
#include <QPushButton>

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
    explicit Board(int server, QWidget *parent = 0);
    ~Board();

signals:
    void decide(int r, int c);

public slots:
    void playerReady();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int isServer;
    int meReady, heReady;
    int needHint, needPlay;
    int mouseEnable;
    int noPlace;
    int currentPaintLevel;
    int userColor;
    int currentColor;
    int TOTALSIZE, BARGIN, PIECEBARGIN;
    int CELLSIZE, GAPSIZE, PIECEOFFSET, x0, y0;
    CellState bdState[64];

    CellLabel *cellArray[64];

    void initResources();
    void initLabels();
    void initInfo();

    QPushButton *startButton;
    int scoreAX0, scoreAY0, scoreBX0, scoreBY0, scoreW, scoreH;
    QLabel *scoreLabelA, *scoreLabelB;
    int infoAX0, infoAY0, infoBX0, infoBY0;
    QLabel *pieceInfoA, *pieceInfoB;

    void paintCurrentColor();
    void paintPieces(int hint, int play);
    void paintScore();

    QColor penColor, bkgColor, cellColorA, cellColorB, pieceColorW, pieceColorB;

    void trySetPiece(int r, int c);
    void hintPieces();

    void gamePrepare();
    void gameStart();
    void gameEnd(int msg);

    QPoint mousePressPos;
    QPoint mousePrevCell, mouseCell;

    QPoint getMouseCell(QPoint pos);

    OAlgo *algo;
    QImage uiWindow, uiPieceB, uiPieceW;
};

#endif // BOARD_H
