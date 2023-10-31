#include <stdio.h>
#include "game.h"

int main(void) {
    Board board = board_new();
    board[10] = piece_new(BLACK, PAWN);
    board[1] = piece_new(WHITE, KNIGHT);
    board_print(board);
    printf("%d\n", piece_can_move(&board[10], 1, board));
}

