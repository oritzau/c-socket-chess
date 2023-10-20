#include "board.h"
#include <stddef.h>
void print(Board board) {
}

Board * board_new() {
    Board board;
}

// this is evil and bad.
// find a better way or test A LOT so it doesn't bite you in the ass later
unsigned char get_piece_index(Piece * piece, Board * board) {
    unsigned char offset = (unsigned char *)piece - (unsigned char *)board;
    return (unsigned char)(offset / sizeof(Piece));
}
