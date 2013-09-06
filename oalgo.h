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

private:
    int user;
    Board *bd;

    int inBoard(int x);
    CellState anti(int clr);
    CellState same(int clr);
    CellState sameCan(int clr);
};

#endif // OALGO_H
