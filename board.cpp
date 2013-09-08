#include "board.h"
#include "oalgo.h"
#include "choosecolor.h"

#include <QPainter>
#include <QDebug>
#include <QImageReader>
#include <QApplication>
#include <QPushButton>

CellLabel::CellLabel(Board *bd, QWidget *parent) :
    QLabel(parent)
{
    father = bd;
    setMouseTracking(true);
}

void CellLabel::mouseMoveEvent(QMouseEvent *event)
{
    QApplication::sendEvent(father, event);
}

void CellLabel::mousePressEvent(QMouseEvent *event)
{
    QApplication::sendEvent(father, event);
}

void CellLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::sendEvent(father, event);
}

Board::Board(int server, QWidget *parent) :
    QWidget(parent),
    CELLSIZE(55), GAPSIZE(1), x0(183), y0(144), PIECEOFFSET(4), TOTALSIZE(8 * 55 + 9 * 1),
    isServer(server), onConnection(0),
    userColor(-1), currentColor(BLACK),
    needHint(0), needPlay(0),
    penColor(0, 0, 0), bkgColor(220, 220, 220),
    cellColorA(170, 170, 170), cellColorB(85, 85, 85),
    pieceColorB(Qt::black), pieceColorW(Qt::white),
    mousePrevCell(-1, -1), mouseCell(-1, -1), mouseEnable(1),
    scoreAX0(103), scoreAY0(482), scoreBX0(753), scoreBY0(482), scoreH(15), scoreW(50),
    infoAX0(92), infoAY0(305), infoBX0(739), infoBY0(305),
    meReady(0), heReady(0)
{
    initResources();
    initLabels();
    initInfo();

    algo = new OAlgo(this);

    setMinimumSize(uiWindow.size());
    setMaximumSize(uiWindow.size());
    setMouseTracking(true);

    connect(startButton, SIGNAL(clicked()), this, SLOT(playerReady()));
    qDebug("ready connected");

    memset(bdState, 0, sizeof(CellState) * 64);

    paintPieces(0, 0);
    paintScore();
    update();
}

void Board::gameEstab()
{
    onConnection = 1;
    roleA->setPixmap(QPixmap("://ui/role.jpg"));
}

void Board::gamePrepare()
{
    qDebug("we'are preparing");
    if (isServer)
    {
        ChooseColor dlg(this);
        userColor = dlg.exec();
        qDebug("%d", userColor);
        if (userColor == BLACK)
            emit decide(GAME_INFO, WHITE);
        else
            emit decide(GAME_INFO, BLACK);
        gameStart();
    }
}

void Board::gameStart()
{
    memset(bdState, 0, sizeof(CellState) * 64);
    paintPieces(0, 0);
    paintScore();
    update();

    qDebug() << "start...";
    algo->reStart();
    algo->setUserC(userColor);
    currentColor = BLACK;

    algo->setPiece(BLACK, 3, 3);
    algo->setPiece(WHITE, 3, 4);
    algo->setPiece(BLACK, 4, 4);
    algo->setPiece(WHITE, 4, 3);

    qDebug() << "set...";
    paintPieces(0, 0);
    paintScore();
    update();
}

void Board::gameEnd(int msg)
{
    qDebug("game end %d", msg);
    startButton->setEnabled(true);
    //emit gameEnded(msg);
}

void Board::playerReady()
{
    meReady = 1;
    emit decide(GAME_INFO, GAME_READY);
    qDebug("send ready");
    if (heReady)
        gamePrepare();
    startButton->setEnabled(false);
}

Board::~Board()
{
    delete algo;
}

void Board::initInfo()
{
    scoreLabelA = new QLabel(this);
    scoreLabelB = new QLabel(this);

    QPalette pal;
    pal.setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));
    pal.setColor(QPalette::WindowText, QColor(216, 156, 48));

    QFont font;
    font.setFamily("arial");
    font.setBold(true);
    font.setPointSize(15);
    scoreLabelA->setGeometry(scoreAX0, scoreAY0, scoreW, scoreH);
    scoreLabelB->setGeometry(scoreBX0, scoreBY0, scoreW, scoreH);
    scoreLabelA->setText("");
    scoreLabelB->setText("");
    scoreLabelA->setPalette(pal);
    scoreLabelB->setPalette(pal);
    scoreLabelA->setFont(font);
    scoreLabelB->setFont(font);

    QPixmap icoPm("://ui/start.png");
    QIcon ico(icoPm);
    startButton = new QPushButton(ico, "", this);
    startButton->setFlat(true);
    startButton->setGeometry(356, 660, 94, 36);
    startButton->setIcon(ico);
    startButton->setIconSize(icoPm.size());

    pieceInfoA = new QLabel(this);
    pieceInfoB = new QLabel(this);
    pieceInfoA->setGeometry(infoAX0, infoAY0, 46, 46);
    pieceInfoB->setGeometry(infoBX0, infoBY0, 46, 46);

    roleA = new QLabel(this);
    roleB = new QLabel(this);
    roleA->setGeometry(24, 194, 116, 161);
    roleB->setGeometry(672, 194, 116, 161);
    roleB->setPixmap(QPixmap("://ui/role.jpg"));
}

void Board::initResources()
{
    QImageReader reader;
    reader.setFileName("://ui/baiqi.tga");
    uiPieceW = reader.read();
    reader.setFileName("://ui/heiqi.tga");
    uiPieceB = reader.read();
    reader.setFileName("://ui/window.jpg");
    uiWindow = reader.read();
}

void Board::initLabels()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            //cellArray[CELL(i, j)] = new QLabel(this);
            cellArray[CELL(i, j)] = new CellLabel(this, this);
            cellArray[CELL(i, j)]->resize(CELLSIZE - 2 * PIECEOFFSET,
                                          CELLSIZE - 2 * PIECEOFFSET);
            cellArray[CELL(i, j)]->move(x0 + GAPSIZE + i * (GAPSIZE + CELLSIZE) + PIECEOFFSET,
                                        y0 + GAPSIZE + j * (GAPSIZE + CELLSIZE) + PIECEOFFSET);
            cellArray[CELL(i, j)]->setAlignment(Qt::AlignCenter);
            cellArray[CELL(i, j)]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
}

void Board::paintScore()
{
    int countA = 0, countB = 0; // B for user
    for (int k = 0; k < 64; k++)
        if (othello::sameClr(bdState[k], userColor))
            countB++;
        else if (othello::antiClr(bdState[k], userColor))
            countA++;
    char tmp[10];
    sprintf(tmp, "%d", countA);
    scoreLabelA->setText(tmp);
    sprintf(tmp, "%d", countB);
    scoreLabelB->setText(tmp);
}

void Board::paintPieces(int hint = 0, int play = 0)
{
    //qDebug() << "hello";
    for (int k = 0; k < 64; k++)
        if (othello::sameClr(bdState[k], BLACK))
            cellArray[k]->setPixmap(QPixmap::fromImage(uiPieceB));
        else if (othello::sameClr(bdState[k], WHITE))
            cellArray[k]->setPixmap(QPixmap::fromImage(uiPieceW));
        else
            cellArray[k]->clear();
}

void Board::paintCurrentColor()
{
    if (userColor == currentColor)
    {
        if (userColor == BLACK)
            pieceInfoB->setPixmap(QPixmap::fromImage(uiPieceB));
        else
            pieceInfoB->setPixmap(QPixmap::fromImage(uiPieceW));
        pieceInfoA->clear();
    }
    else
    {
        if (userColor == BLACK)
            pieceInfoA->setPixmap(QPixmap::fromImage(uiPieceW));
        else
            pieceInfoA->setPixmap(QPixmap::fromImage(uiPieceB));
        pieceInfoB->clear();
    }
}

void Board::paintEvent(QPaintEvent *)
{
    //qDebug() << "hi";
    // only paint board & cursor
    QPainter p(this);

    p.drawImage(0, 0, uiWindow);

    int xx = x0 + GAPSIZE, yy = y0 + GAPSIZE;
    int i = mouseCell.x(), j = mouseCell.y();
    if (i >= 0 && j >= 0)
    {
        p.setBrush(Qt::NoBrush);
        QPen penC(Qt::red);
        if ((bdState[CELL(i, j)] & IS_PIECE) == 0 &&
                othello::canPut(bdState[CELL(i, j)], currentColor))
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
    QPoint pos = mapFromGlobal(event->globalPos());
    if (!mouseEnable) return;
    //qDebug("cap!");
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(pos);
    //qDebug("%d %d", mouseCell.x(), mouseCell.y());
    if (mouseCell != mousePrevCell)
        update();
}

void Board::mousePressEvent(QMouseEvent *event)
{
    //if (userColor != currentColor) return;
    QPoint pos = mapFromGlobal(event->globalPos());
    mousePressPos = pos;
    mousePrevCell = mouseCell;
    mouseCell = getMouseCell(pos);
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    //if (userColor != currentColor) return;
    QPoint pos = mapFromGlobal(event->globalPos());
    if (pos != mousePressPos)
        return;
    trySetPiece(mouseCell.x(), mouseCell.y());
}

void Board::trySetPiece(int r, int c)
{
    int x = CELL(r, c);
    if ((bdState[x] & IS_PIECE) != 0) return;
    if (!othello::canPut(bdState[x], currentColor))
    {
        //hintPiece();
        return;
    }

    if (algo->setPiece(currentColor, r, c) != 0)
    {
        noPlace = 0;
        currentColor = BLACK + WHITE - currentColor;
    }
    else
    {
        if (algo->refreshCan(currentColor) == 0)
            gameEnd(algo->checkWin());
    }
    paintCurrentColor();
    paintPieces();
    paintScore();
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

void Board::react(int r, int c)
{
    qDebug("%d %d", r, c);
    if (r == GAME_INFO)
    {
        switch (c)
        {
        case GAME_READY:
        {
            heReady = 1;
            if (meReady)
                gamePrepare();
            break;
        }
        case BLACK:
        {
            userColor = BLACK;
            gameStart();
            break;
        }
        case WHITE:
        {
            userColor = WHITE;
            gameStart();
            break;
        }
        default:
        {
            return;
        }

        }
    }
}
