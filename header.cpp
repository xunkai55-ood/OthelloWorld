#include "header.h"

namespace othello{

int sameClr(CellState cell, int clr)
{
    if (clr == BLACK)
        if ((cell & IS_BLACK) == IS_BLACK)
            return 1;
        else
            return 0;
    else
        if ((cell & IS_WHITE) == IS_WHITE)
            return 1;
        else
            return 0;
}

int antiClr(CellState cell, int clr)
{
    return sameClr(cell, WHITE + BLACK - clr);
}

CellState canState(int clr)
{
    if (clr == BLACK)
        return CAN_BLACK;
    if (clr == WHITE)
        return CAN_WHITE;
}

int canPut(CellState cell, int clr)
{
    if ((cell & IS_PIECE) != 0) return 0;
    return cell & canState(clr);
}

CellState toState(int clr)
{
    if (clr == BLACK) return IS_BLACK; else return IS_WHITE;
}

}
