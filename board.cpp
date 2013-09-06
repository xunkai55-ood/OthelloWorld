#include "board.h"
#include "oalgo.h"

#include <QPainter>
#include <QDebug>

Board::Board(int userC, QWidget *parent) :
    QWidget(parent),
    CELLSIZE(60), GAPSIZE(5), TOTALSIZE(8 * CELLSIZE + 9 * GAPSIZE), BARGIN(30),
    PIECEBARGIN(10),
    //penColor(153, 51, 0), bkgColor(153, 128, 0),
    penColor(0, 0, 0), bkgColor(220, 220, 220),
    cellColorA(170, 170, 170), cellColorB(85, 85, 85),
    pieceColorB(Qt::black), pieceColorW(Qt::white),
    mousePrevCell(-1, -1), mouseCell(-1, -1), deadStatus(0)
{
    algo = new oAlgo(userC, this);

    userColor = userC;
    userColorLocal = userC;

    setMinimumHeight(TOTALSIZE + BARGIN);
    setMinimumWidth(TOTALSIZE + BARGIN);
    setMouseTracking(true);

    memset(bdState, 0, sizeof(CellState) * 64);
    algo->setPiece(BLACK, 3, 3);
    algo->setPiece(WHITE, 3, 4);
    algo->setPiece(BLACK, 4, 4);
    algo->setPiece(WHITE, 4, 3);

}

Board::~Board()
{
    delete algo;
}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    // down X, right Y
    int leftTopX = x0 = height() / 2 - TOTALSIZE / 2;
    int leftTopY = y0 = width() / 2 - TOTALSIZE / 2;

    // paint bkg
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(bkgColor));
    p.drawRect(leftTopX, leftTopY, TOTALSIZE, TOTALSIZE);

    // paint cells
    leftTopX += GAPSIZE, leftTopY += GAPSIZE;

    p.setPen(QPen(penColor));
    p.setBrush(QBrush(cellColorA));
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2 == 0)
                p.drawRect(leftTopX + i * (GAPSIZE + CELLSIZE),
                           leftTopY + j * (GAPSIZE + CELLSIZE),
                           CELLSIZE, CELLSIZE);
    p.setBrush(QBrush(cellColorB));
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2 == 1)
                p.drawRect(leftTopX + i * (GAPSIZE + CELLSIZE),
                           leftTopY + j * (GAPSIZE + CELLSIZE),
                           CELLSIZE, CELLSIZE);

    // paint pieces
    QPen penS(Qt::black);
    penS.setWidth(3);
    p.setPen(penS);

    for (int k = 0; k < 64; k++)
    {
        if ((bdState[k] & IS_PIECE) == 0) continue;
        int x = (k / 8) * (GAPSIZE + CELLSIZE), y = (k % 8) * (GAPSIZE + CELLSIZE);

        if ((bdState[k] & IS_BLACK) != IS_BLACK)
            p.setBrush(pieceColorB);
        else
            p.setBrush(pieceColorW);
        p.drawEllipse(x + PIECEBARGIN + leftTopX,
                      y + PIECEBARGIN + leftTopY,
                      CELLSIZE - PIECEBARGIN * 2,
                      CELLSIZE - PIECEBARGIN * 2);
    }

    // paint cursor
    int i = mouseCell.x(), j = mouseCell.y();
    if (i >= 0 && j >= 0)
    {
        p.setBrush(Qt::NoBrush);
        QPen penC(Qt::red);
        //qDebug("%d", bdState[CELL(i, j)]);
        if ((bdState[CELL(i, j)] & IS_PIECE) == 0 &&
                othello::canPut(bdState[CELL(i, j)], userColorLocal))
            penC.setColor(Qt::green);
        //else if (bdState[i * 8 + j] == IS_BLACK)
        //    penC.setColor(Qt::black);
        //else if (bdState[i * 8 + j] == IS_WHITE)
        //    penC.setColor(Qt::white);
        penC.setWidth(3);
        p.setPen(penC);
        p.drawRect(i * (GAPSIZE + CELLSIZE) + leftTopX,
                   j * (GAPSIZE + CELLSIZE) + leftTopY,
                   CELLSIZE, CELLSIZE);
    }
}

void Board::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug("cap!");
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(event->pos());
    //qDebug("%d %d", mouseCell.x(), mouseCell.y());
    if (mouseCell != mousePrevCell)
        update();
}

void Board::mousePressEvent(QMouseEvent *event)
{
    mousePressPos = event->pos();
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(event->pos());
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos() != mousePressPos)
        return;
    trySetPiece(mouseCell.x(), mouseCell.y());
}

void Board::trySetPiece(int r, int c)
{
    int x = CELL(r, c);
    if ((bdState[x] & IS_PIECE) != 0) return;
    if (!othello::canPut(bdState[x], userColorLocal)) return;

    if (algo->setPiece(userColorLocal, r, c) != 0)
    {
        deadStatus = 0;
        userColorLocal = BLACK + WHITE - userColorLocal;
    }
    else
    {
        deadStatus += 1;
        if (deadStatus == 1)
            qDebug() << "still you";
        else
            qDebug("win : %d", algo->checkWin());
    }
}

QPoint Board::getMouseCell(QPoint pos)
{
    int x = pos.x(), y = pos.y();

    if (x <= x0 + GAPSIZE || x >= x0 + TOTALSIZE ||
        y <= y0 + GAPSIZE || y >= y0 + TOTALSIZE)
        return QPoint(-1, -1);
    x -= GAPSIZE + x0, y -= GAPSIZE + y0;
    if (x % (GAPSIZE + CELLSIZE) >= CELLSIZE ||
        y % (GAPSIZE + CELLSIZE) >= CELLSIZE)
        return QPoint(-1, -1);
    x = x / (GAPSIZE + CELLSIZE);
    y = y / (GAPSIZE + CELLSIZE);
    return QPoint(x, y);
}

void Board::gameEnd(int msg)
{
    qDebug("game end %d", msg);

    //emit gameEnded(msg);
}
