#include "oalgo.h"
#include "board.h"
#include "header.h"

oAlgo::oAlgo(int userC, Board *bdFather) :
    onBoard(0)
{
    user = userC;
    bd = bdFather;
}

int oAlgo::setPiece(int clr, int r, int c)
{
    onBoard++;
    int dir[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    /* clear shrink level */

    for (int x = 0; x < 64; x++)
        bd->bdState[x] &= ~SET_SHRINK;

    /* calc */
    bd->bdState[CELL(r, c)] = (IS_PIECE | othello::toState(clr) | SET_SHRINK);

    //qDebug("treat %d %d", r, c);
    for (int d = 0; d < 8; d++)
    {
        int ok = 0, i, j, k;
        for (k = 1, i = r + dir[d][0], j = c + dir[d][1];
             inBoard(i) && inBoard(j); k++)
        {
            if (othello::sameClr(bd->bdState[CELL(i, j)], clr))
            {
                //qDebug("find a same %d %d %d", i, j, bd->bdState[CELL(i, j)]);
                ok = 1;
                break;
            }
            else if (!othello::antiClr(bd->bdState[CELL(i, j)], clr))
            {
                //qDebug("not anti %d %d %d", i, j, bd->bdState[CELL(i, j)]);
                break;
            }
            //qDebug("valid %d %d %d", i, j, bd->bdState[CELL(i, j)]);
            i += dir[d][0], j += dir[d][1];
        }
        if (!ok || k == 1) continue;
        i = r + dir[d][0], j = c + dir[d][1];
        for (int kk = 0; kk < k - 1; kk++)
        {
            //qDebug("paint %d %d", i, j);
            bd->bdState[CELL(i, j)] = (IS_PIECE | othello::toState(clr) | SET_SHRINK);
            i += dir[d][0], j += dir[d][1];
        }
    }

    clearCan();
    //refreshCan(BLACK + WHITE - user);
    //return refreshCan(user);

    refreshCan(clr);
    return refreshCan(BLACK + WHITE - clr);
}

void oAlgo::clearCan()
{
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if ((bd->bdState[CELL(r, c)] & IS_PIECE) == 0)
                bd->bdState[CELL(r, c)] = 0;
}

int oAlgo::refreshCan(int clr)
{
    int dir[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    /* clear */
    int rst = 0;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
        {
            if ((bd->bdState[CELL(r, c)] & IS_PIECE) == 0)
            {
                int flag = 0;
                for (int d = 0; d < 8; d++)
                {
                    int ok = 0, i, j, k;
                    for (k = 1, i = r + dir[d][0], j = c + dir[d][1];
                         inBoard(i) && inBoard(j); k++)
                    {
                        if (othello::sameClr(bd->bdState[CELL(i, j)], clr))
                        {
                            ok = 1;
                            break;
                        }
                        else if (!othello::antiClr(bd->bdState[CELL(i, j)], clr))
                        {
                            break;
                        }
                        i += dir[d][0], j += dir[d][1];
                    }
                    if (!ok || k == 1) continue;
                    flag = 1;
                }
                if (flag)
                {
                    bd->bdState[CELL(r, c)] |= othello::canState(clr);
                    rst++;
                }
            }
        }
    return rst;
}

int oAlgo::checkWin()
{
    int s = 0;
    for (int i = 0; i < 64; i++)
        if (othello::sameClr(bd->bdState[i], user))
            s++;
    if (s * 2 > onBoard) return GAME_WIN;
    if (s * 2 == onBoard) return GAME_TIE;
    return GAME_LOSE;
}

int oAlgo::inBoard(int x)
{
    return (x >= 0 && x < 8);
}


