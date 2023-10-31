#include "game.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

Board board_new() {
    Board board = calloc(BOARD_SIZE, sizeof(Piece *));
    return board;
}

Piece * piece_new(PieceColor color, PieceKind kind) {
    Piece * piece = (Piece *)malloc(sizeof(Piece));
    piece->kind = kind;
    piece->color = color;
    piece->has_moved = 0;
    return piece;
}

void print_char(char c) {
    printf("%3c", c);
}

void board_print(Board board) {
    int row_number, column_number, start;
    Piece * current = NULL;
    for (row_number = 1; row_number <= 8; row_number++) {
        start = BOARD_SIZE - (row_number * 8);
        for (column_number = start; column_number < start + 8; column_number++) {
            current = board[column_number];
            if (current == NULL) {
                print_char('_');
            } else {
                switch(current->kind) {
                case PAWN:
                    print_char('P');
                    break;
                case KNIGHT:
                    print_char('N');
                    break;
                case BISHOP:
                    print_char('B');
                    break;
                case ROOK:
                    print_char('R');
                    break;
                case QUEEN:
                    print_char('Q');
                    break;
                case KING:
                    print_char('K');
                    break;
                }
            }
        }
        printf("\n");
    }
}

int get_piece_index(Piece ** piece, Board board) {
    return (int)(piece - board);
}

// can't be called with null
// probably not gonna add en passant for a bit if at all
int pawn_can_move(Piece * pawn, int current_index, int target_index, Board board) {
    int current_row = (int)(current_index / ROW_SIZE);
    int current_col = current_index % COL_SIZE;
    int target_row = (int)(target_index / ROW_SIZE);
    int target_col = target_index % COL_SIZE;

    int valid_column = 0;
    int valid_row = 0;
    printf("Current: (%d, %d) -> Target: (%d, %d)\n", 
            current_row, current_col, target_row, target_col);

    int direction_modifier = pawn->color == WHITE ? 1 : -1;
    int range = (pawn->has_moved ? 2 : 1) * direction_modifier;

    // target tile occupied by opposite color piece
    if (board[target_index] != NULL && board[target_index]->color != pawn->color) {
        valid_column = target_col == current_col + 1 
            || target_col == current_col - 1;
        valid_row = target_row == current_row + (1 * direction_modifier);
        return valid_row && valid_column;
    }

    // intending to move 1 square, no need to check has_moved
    if ((current_row - target_row) * direction_modifier == 1) {
        return board[target_index] == NULL;
    }

    // TODO: 2 square moves (en passant)
    return 0;
}

// only checks for physically able, meaning accounts for piece
// positions, piece colors, but NOT CHECKS
int piece_can_move(Piece ** piece, int target_index, Board board) {
    if (piece == NULL)
        return NULL_PIECE_ERROR;
    int current_index = get_piece_index(piece, board);
    Piece * active_piece = *piece;
    switch (active_piece->kind) {
    case PAWN:
        return pawn_can_move(active_piece, current_index, target_index, board);
    case KNIGHT:
    case BISHOP:
    case ROOK:
    case QUEEN:
    case KING:
        return 0;
    }
}
