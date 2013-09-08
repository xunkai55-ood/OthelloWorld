#ifndef HEADER_H
#define HEADER_H

#define MODE_NONE (0)
#define MODE_CLIENT (1)
#define MODE_SERVER (2)

#define BLACK (1)
#define WHITE (2)

#define CELL(r, c) (((r) * (8)) + (c))

typedef int CellState;

// the highest 3 bit is for label
// 1....... has piece
// 0....... no piece
// .1...... black
// ..1..... white

#define IS_PIECE (0x80)
#define IS_BLACK (0xC0)
#define IS_WHITE (0xA0)
#define CAN_BLACK (0x40)
#define CAN_WHITE (0x20)
#define IS_SHRINK (0x8f)
#define SET_SHRINK (0x0f)

namespace othello{

int sameClr(CellState cell, int clr);
int antiClr(CellState cell, int clr);
CellState canState(int clr);
int canPut(CellState cell, int clr);
CellState toState(int clr);

}

#define GAME_WIN (13)
#define GAME_LOSE (11)
#define GAME_TIE (12)

#define GAME_INFO (-5)
#define GAME_RUNAWAY (-10)
#define GAME_READY (-1)
#define GAME_FATALERROR (-9)

#endif // HEADER_H
