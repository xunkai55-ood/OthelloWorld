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

    int checkWin();

private:
    int user;
    Board *bd;
    int onBoard;

    int inBoard(int x);
};

#endif // OALGO_H
