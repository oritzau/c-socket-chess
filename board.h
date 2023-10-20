#ifndef BOARDH
#define BOARDH
#include "common.h"
#include "piece.h"

typedef struct Piece Board[64]; 
Board * board_new();

unsigned char get_piece_index(Piece * piece, Board * board);
#endif
