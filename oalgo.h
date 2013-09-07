#ifndef OAlgo_H
#define OAlgo_H

#include "header.h"

class Board;

class OAlgo
{
public:
    OAlgo(Board *bd);

    void reStart();
    void setUserC(int clr);
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

#endif // OAlgo_H
