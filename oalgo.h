#ifndef OALGO_H
#define OALGO_H

#include "header.h"

class Board;

class oAlgo
{
public:
    oAlgo(int userC, Board *bd);

    int setPiece(int clr, int r, int c);
    int refreshCan(int clr);
    void clearCan();

    void setWin(int x);
    int checkWin();

private:
    int user;
    Board *bd;
    int win;
    int onBoard;

    int inBoard(int x);
    inline CellState anti(int clr);
    inline CellState same(int clr);
    inline CellState sameCan(int clr);
};

#endif // OALGO_H
