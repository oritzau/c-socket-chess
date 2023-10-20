#ifndef PIECEH
#define PIECEH

#include <stdint.h>
enum PieceKind {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
};

enum PieceColor {
    WHITE = 0,
    BLACK = 1,
};

typedef struct Piece {
    enum PieceColor color;
    enum PieceKind kind;
} Piece;

typedef struct Move {
} Move;


#endif
