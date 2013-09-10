#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QLabel>
#include <QTimer>
#include <QPushButton>

#include "header.h"
#include "extimer.h"
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
    void gamePrepare();
    void gameEstab();
    void react(int r, int c);

    void hintPieces();
    void surrender();
    void userTimeOut();

    void hintStop();
    void movieStop();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int isServer, onConnection, inGame;
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

    QPushButton *startButton, *surrenderButton, *hintButton;
    int scoreAX0, scoreAY0, scoreBX0, scoreBY0, scoreW, scoreH;
    QLabel *scoreLabelA, *scoreLabelB;
    int infoAX0, infoAY0, infoBX0, infoBY0;
    QLabel *pieceInfoA, *pieceInfoB;
    QLabel *roleA, *roleB;

    void paintCurrentColor();
    void paintPieces(int hint, int play);
    void paintScore();

    QColor penColor, bkgColor, cellColorA, cellColorB, pieceColorW, pieceColorB;

    void trySetPiece(int r, int c);

    void gameStart();
    void gameEnd(int msg);
    void gameMsg(const char *s);

    QPoint mousePressPos;
    QPoint mousePrevCell, mouseCell;

    QPoint getMouseCell(QPoint pos);

    OAlgo *algo;
    QImage uiWindow, uiPieceB, uiPieceW;

    ExTimer *restA, *restB, *totalA, *totalB;
    QLabel *rTA, *rTB, *tTA, *tTB;

    QTimer *hintTimer, *movieTimer;

    void moviePlay();

    std::vector<int> hintV;
    std::vector<int> movieV;
    int inHint, inMovie;
};

#endif // BOARD_H
