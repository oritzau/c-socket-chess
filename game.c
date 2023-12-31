#include "game.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

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

Piece piece_copy(Piece *piece) {
    Piece p;
    p.kind = piece->kind;
    p.has_moved = piece->has_moved;
    p.color = piece->color;
    return p;
}

void print_char(char c) {
    printf("%3c", c);
}

// this allocates 3B of memory, remember to free after printing if this gets used
char * tile_index_to_name(int index) {
    int row = (int)(index / ROW_SIZE);
    int col = index % ROW_SIZE;
    char * name = malloc(sizeof(char) * 3);
    name[0] = 'A' + col;
    name[1] = '1' + row;
    name[2] = '\0';
    return name;
}

int tile_name_to_index(char * tile_name) {
    int row = '1' - tile_name[1];
    int col = 'A' - tile_name[0];
    return -1 * (row * ROW_SIZE + col);
}

void board_print(Board board) {
    int row_number, column_number, start;
    Piece * current = NULL;
    for (row_number = 1; row_number <= ROW_SIZE; row_number++) {
        start = BOARD_SIZE - (row_number * ROW_SIZE);
        for (column_number = start; column_number < start + ROW_SIZE; column_number++) {
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

Piece * board_index_by_row_col(Board board, int row, int col) {
    return board[row * 8 + col];
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
    char * current_tile = tile_index_to_name(current_index);
    char * target_tile = tile_index_to_name(target_index);
    current_tile = target_tile = NULL;
    free(current_tile);
    free(target_tile);
    int direction_modifier = pawn->color == WHITE ? 1 : -1;

    // attacking
    if (board[target_index] != NULL && board[target_index]->color != pawn->color) {
        valid_column = target_col == current_col + 1 
            || target_col == current_col - 1;
        valid_row = target_row == current_row + (1 * direction_modifier);
        return valid_row && valid_column;
    }

    // intending to move 1 square, no need to check has_moved
    if ((target_row - current_row) * direction_modifier == 1) {
        return board[target_index] == NULL;
    }

    if ((target_row - current_row) * direction_modifier == 2 && !pawn->has_moved) {
        // tile 1 in front not occupied
        int tile_ahead_index = (current_row + 1 * direction_modifier) * direction_modifier * ROW_SIZE + target_col;
        int space_ahead_empty = board[tile_ahead_index] == NULL;
        return space_ahead_empty && board[target_index] == NULL;
    }
    return 0;
}

int knight_can_move(Piece * knight, int current_index, int target_index, Board board) {
    int l_shape = 0;
    int current_row = (int)(current_index / ROW_SIZE);
    int current_col = current_index % COL_SIZE;
    int target_row = (int)(target_index / ROW_SIZE);
    int target_col = target_index % COL_SIZE;

    int row_diff = abs(target_row - current_row);
    int col_diff = abs(target_col - current_col);
    l_shape = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
    return l_shape && (board[target_index] == NULL || board[target_index]->color != knight->color);
}

int bishop_can_move(Piece * bishop, int current_index, int target_index, Board board) {
    if (current_index == target_index)
        return 0;
    int current_row = (int)(current_index / ROW_SIZE);
    int current_col = current_index % COL_SIZE;
    int target_row = (int)(target_index / ROW_SIZE);
    int target_col = target_index % COL_SIZE;


    int row_diff = target_row - current_row;
    int col_diff = target_col - current_col;

    int horizontal_direction_modifier = col_diff > 0 ? 1 : -1;
    int vertical_direction_modifier = row_diff > 0 ? 1 : -1;

    // not diagonal
    if (abs(row_diff) != abs(col_diff))
        return 0;
    
    int temp_row = current_row;
    int temp_col = current_col;
    while (temp_col != target_col && temp_row != target_row) {
        temp_col = temp_col + 1 * horizontal_direction_modifier;
        temp_row = temp_row + 1 * vertical_direction_modifier;
        // location is not null, meaning a piece is already on a square the bishop is trying to cross
        if (board_index_by_row_col(board, temp_row, temp_col)) {
            return 0;
        }
    }
    return board[target_index] == NULL || board[target_index]->color != bishop->color;
}
int rook_can_move(Piece * rook, int current_index, int target_index, Board board) {
    if (target_index == current_index)
        return 0;
    int current_row = (int)(current_index / ROW_SIZE);
    int current_col = current_index % COL_SIZE;
    int target_row = (int)(target_index / ROW_SIZE);
    int target_col = target_index % COL_SIZE;

    int row_diff = target_row - current_row;
    int col_diff = target_col - current_col;

    int moving_horizontal = abs(col_diff) ? 1 : 0;
    int moving_vertical = abs(row_diff) ? 1: 0;

    int horizontal_direction_modifier = col_diff > 0 ? 1 : -1;
    int vertical_direction_modifier = row_diff > 0 ? 1 : -1;

    // not moving strictly vertically or strictly horizontal
    if (moving_vertical && moving_horizontal) 
        return 0;
    int temp_row = current_row;
    int temp_col = current_col;
    while (temp_col != target_col || temp_row != target_row) {
        if (moving_vertical) {
            temp_row += 1 * vertical_direction_modifier;
        } else {
            temp_col += 1 * horizontal_direction_modifier;
        }
        if (board_index_by_row_col(board, temp_row, temp_col))
            return 0;
    }
    return board[target_index] == NULL || board[target_index]->color != rook->color;;
}

int queen_can_move(Piece * queen, int current_index, int target_index, Board board) {

    queen->kind = ROOK;
    int rook_can_move = piece_can_move(&board[current_index], target_index, board);
    queen->kind = BISHOP;
    int bishop_can_move = piece_can_move(&board[current_index], target_index, board);
    queen->kind = QUEEN;
    return rook_can_move || bishop_can_move;
}

int king_can_move(Piece * king, int current_index, int target_index, Board board) {
    int current_row = (int)(current_index / ROW_SIZE);
    int current_col = current_index % COL_SIZE;
    int target_row = (int)(target_index / ROW_SIZE);
    int target_col = target_index % COL_SIZE;

    int row_diff = target_row - current_row;
    int col_diff = target_col - current_col;

    int valid_horizontal = (row_diff == 0 && abs(col_diff) == 1);
    int valid_vertical = (abs(row_diff) == 1 && col_diff == 0);
    int valid_diagonal = (abs(row_diff) == 1 && abs(col_diff) == 1);

    return (valid_vertical || valid_horizontal || valid_diagonal) && (board[target_index] == NULL || board[target_index]->color != king->color); 
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
        return knight_can_move(active_piece, current_index, target_index, board);
    case BISHOP:
        return bishop_can_move(active_piece, current_index, target_index, board);
    case ROOK:
        return rook_can_move(active_piece, current_index, target_index, board);
    case QUEEN:
        return queen_can_move(active_piece, current_index, target_index, board);
    case KING:
        return king_can_move(active_piece, current_index, target_index, board);
    }
    return 0;
}
