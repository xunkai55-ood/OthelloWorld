#ifndef OALGO_H
#define OALGO_H

#include "header.h"

class Board;

class oAlgo
{
public:
    oAlgo(int userC, Board *bd);

    int setPiece(int clr, int r, int c);
    void refreshCan(int clr);
    void clearCan();

private:
    int user;
    Board *bd;

    int inBoard(int x);
    inline CellState anti(int clr);
    inline CellState same(int clr);
    inline CellState sameCan(int clr);
};

#endif // OALGO_H
