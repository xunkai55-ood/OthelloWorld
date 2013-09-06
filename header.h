#ifndef HEADER_H
#define HEADER_H

#define MODE_NONE 0
#define MODE_CLIENT 1
#define MODE_SERVER 2

#define BLACK 1
#define WHITE 2

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
#define GET_SHRINK (0x1f)


#endif // HEADER_H
