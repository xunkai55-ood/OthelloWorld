#include "board.h"
#include "oalgo.h"

#include <QPainter>
#include <QDebug>
#include <QImageReader>

Board::Board(int userC, QWidget *parent) :
    QWidget(parent),
    //CELLSIZE(60), GAPSIZE(5), TOTALSIZE(8 * CELLSIZE + 9 * GAPSIZE), BARGIN(30),
    //PIECEBARGIN(10),
    CELLSIZE(55), GAPSIZE(1), x0(183), y0(144), PIECEOFFSET(4),
    TOTALSIZE(8 * 55 + 9 * 1),
    //penColor(153, 51, 0), bkgColor(153, 128, 0),
    penColor(0, 0, 0), bkgColor(220, 220, 220),
    cellColorA(170, 170, 170), cellColorB(85, 85, 85),
    pieceColorB(Qt::black), pieceColorW(Qt::white),
    mousePrevCell(-1, -1), mouseCell(-1, -1), mouseEnable(1)
{
    QImageReader reader;
    reader.setFileName("://ui/baiqi.tga");
    uiPieceW = reader.read();
    reader.setFileName("://ui/heiqi.tga");
    uiPieceB = reader.read();
    reader.setFileName("://ui/window.jpg");
    uiWindow = reader.read();

    algo = new oAlgo(userC, this);

    userColor = userC;
    userColorLocal = userC;

    setMinimumSize(uiWindow.size());
    setMaximumSize(uiWindow.size());
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

void Board::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.drawImage(0, 0, uiWindow);

    int xx = x0 + GAPSIZE, yy = y0 + GAPSIZE;
    for (int k = 0; k < 64; k++)
    {
        int x = (k / 8) * (GAPSIZE + CELLSIZE) + xx;
        int y = (k % 8) * (GAPSIZE + CELLSIZE) + yy;
        if (othello::sameClr(bdState[k], BLACK))
        {
            p.drawImage(x + PIECEOFFSET, y + PIECEOFFSET, uiPieceB);
        }
        else if (othello::sameClr(bdState[k], WHITE))
        {
            p.drawImage(x + PIECEOFFSET, y + PIECEOFFSET, uiPieceW);
        }
    }

    int i = mouseCell.x(), j = mouseCell.y();
    if (i >= 0 && j >= 0)
    {
        p.setBrush(Qt::NoBrush);
        QPen penC(Qt::red);
        if ((bdState[CELL(i, j)] & IS_PIECE) == 0 &&
                othello::canPut(bdState[CELL(i, j)], userColorLocal))
            penC.setColor(Qt::green);
        penC.setWidth(3);
        p.setPen(penC);
        p.drawRect(i * (GAPSIZE + CELLSIZE) + xx,
                   j * (GAPSIZE + CELLSIZE) + yy,
                   CELLSIZE, CELLSIZE);
    }
}

void Board::mouseMoveEvent(QMouseEvent *event)
{
    if (!mouseEnable) return;
    //qDebug("cap!");
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(event->pos());
    //qDebug("%d %d", mouseCell.x(), mouseCell.y());
    if (mouseCell != mousePrevCell)
        update();
}

void Board::mousePressEvent(QMouseEvent *event)
{
    if (!mouseEnable) return;
    mousePressPos = event->pos();
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(event->pos());
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    if (!mouseEnable) return;
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
        noPlace = 0;
        userColorLocal = BLACK + WHITE - userColorLocal;
    }
    else
    {
        if (algo->refreshCan(userColorLocal) != 0)
            qDebug("still you");
        else
        {
            int isWin = algo->checkWin();
            qDebug("win? %d", isWin);
        }
    }
    update();
}

QPoint Board::getMouseCell(QPoint pos)
{
    int x = pos.x(), y = pos.y();
    //qDebug("%d %d", x, y);
    //qDebug("gap %d cell %d total %d x0 %d y0 %d",
    //       GAPSIZE, CELLSIZE, TOTALSIZE, x0, y0);

    if (x <= x0 + GAPSIZE || x >= x0 + TOTALSIZE ||
        y <= y0 + GAPSIZE || y >= y0 + TOTALSIZE)
        return QPoint(-1, -1);
    x -= GAPSIZE + x0, y -= GAPSIZE + y0;
    if (x % (GAPSIZE + CELLSIZE) >= CELLSIZE ||
        y % (GAPSIZE + CELLSIZE) >= CELLSIZE)
        return QPoint(-1, -1);
    x = x / (GAPSIZE + CELLSIZE);
    y = y / (GAPSIZE + CELLSIZE);
    //qDebug("cell : %d %d", x, y);
    return QPoint(x, y);
}

void Board::gameEnd(int msg)
{
    qDebug("game end %d", msg);

    //emit gameEnded(msg);
}
