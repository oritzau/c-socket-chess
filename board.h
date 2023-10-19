#ifndef BOARDH
#define BOARDH
#include "common.h"
#include "piece.h"

typedef Piece * Board[8][8]; // alias 64 piece pointers in an array as a Board
Board * board_new();
#endif
