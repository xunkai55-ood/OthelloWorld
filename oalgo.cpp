#include "oalgo.h"
#include "board.h"

oAlgo::oAlgo(int userC, Board *bdFather)
{
    user = userC;
    bd = bdFather;
}

int oAlgo::setPiece(int clr, int r, int c)
{
    int dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    /* clear shrink level */

    for (int x = 0; x < 64; x++)
        bd->bdState[x] &= ~GET_SHRINK;

    /* calc */

    int level = 0;
    int q[65][2];
    int hd = 0, tl = 1;
    q[hd][0] = r, q[hd][1] = c;

    while (++level)
    {
        int newTl = tl;
        while (hd < tl)
        {
            int i0 = q[hd][0], j0 = q[hd][1];
            hd++;
            for (int d = 0; d < 4; d++)
            {
                int ok = 0, i, j, k;
                for (k = 1, i = i0 + dir[d][0], j = j0 + dir[d][1];
                     inBoard(i) && inBoard(j); k++, i += dir[d][0], j += dir[d][1])
                    if (bd->bdState[CELL(i, j)] == same(clr))
                    {
                        ok = 1;
                        break;
                    }
                    else if (bd->bdState[CELL(i, j)] != anti(clr))
                    {
                        break;
                    }
                if (!ok || k == 1) continue;
                for (int kk = 1, i = i0 + dir[d][0], j = j0 + dir[d][1]; kk < k;
                     kk++, i += dir[d][0], j += dir[d][1]);
                {
                    bd->bdState[CELL(i, j)] = IS_PIECE | same(clr) | level;
                    q[newTl][0] = i;
                    q[newTl][1] = j;
                    newTl++;
                }
            }
        }
        if (tl == newTl)
        {
            level--;
            break;
        }
        tl = newTl;
    }
    refreshCan(BLACK);
    refreshCan(WHITE);
    return level;
}

void oAlgo::refreshCan(int clr)
{
    int dir[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    qDebug("refreshing...");
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
        {
            //qDebug("refreshing...%d %d %d", r, c, bd->bdState[CELL(r, c)]);

            if ((bd->bdState[CELL(r, c)] & IS_PIECE) == 0)
            {
                qDebug("refreshing...%d %d", r, c);
                int flag = 0;
                for (int d = 0; d < 4; d++)
                {
                    int ok = 0, i, j, k;
                    for (k = 1, i = r + dir[d][0], j = c + dir[d][1];
                         inBoard(i) && inBoard(j); k++, i += dir[d][0], j += dir[d][1])
                        if (bd->bdState[CELL(i, j)] == same(clr))
                        {
                            ok = 1;
                            break;
                        }
                        else if (bd->bdState[CELL(i, j)] != anti(clr))
                        {
                            break;
                        }
                    if (!ok || k == 1) continue;
                    flag = 1;
                }
                if (flag)
                    bd->bdState[CELL(r, c)] |= sameCan(clr);
            }
        }
}

int oAlgo::inBoard(int x)
{
    return (x >= 0 && x < 8);
}

CellState oAlgo::anti(int clr)
{
    if (clr == BLACK)
        return IS_WHITE;
    if (clr == WHITE)
        return IS_BLACK;
}

CellState oAlgo::same(int clr)
{
    if (clr == BLACK)
        return IS_BLACK;
    if (clr == WHITE)
        return IS_WHITE;
}

CellState oAlgo::sameCan(int clr)
{
    if (clr == BLACK)
        return CAN_BLACK;
    if (clr == WHITE)
        return CAN_WHITE;
}
