#ifndef PIECEH
#define PIECEH

#include <stdint.h>
enum PieceKind {
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
};

typedef struct Piece {
    uint8_t color;
    enum PieceKind kind;
} Piece;

#endif
