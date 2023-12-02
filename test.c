#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "minunit.h"

static Board board;

void test_setup(void) {
    board = board_new();
}

void test_teardown(void) {
    free(board);
    board = NULL;
}

// utils tests 
MU_TEST(test_tile_name_to_indexA1) {
    int index = tile_name_to_index("A1");
    mu_assert_int_eq(0, index);
}

MU_TEST(test_tile_name_to_indexE4) {
    int index = tile_name_to_index("E4");
    mu_assert_int_eq(28, index);
}

MU_TEST(test_index_to_tile_nameD2) {
    char * name = tile_index_to_name(11);
    mu_assert_string_eq("D2", name);
}

MU_TEST(test_get_by_row_col) {
    board[tile_name_to_index("E2")] = piece_new(WHITE, QUEEN);
    mu_assert(board_index_by_row_col(board, 1, 4) != NULL, "board at 1, 4 (e2) should have a queen on it");
}

// pawn tests
MU_TEST(test_white_pawn_move1turn1) {
    board[tile_name_to_index("E2")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E2")], 
            tile_name_to_index("E3"), 
            board
        );
    mu_assert(can_move, "White pawn should be able to go E2->E4");
}

MU_TEST(test_white_pawn_move2turn1) {
    board[tile_name_to_index("E2")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E2")], 
            tile_name_to_index("E4"), 
            board
        );
    mu_assert(can_move, "White pawn should be able to go E2->E4");
}

MU_TEST(test_black_pawn_move1turn1) {
    board[tile_name_to_index("E7")] = piece_new(BLACK, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E7")], 
            tile_name_to_index("E6"), 
            board
        );
    mu_assert(can_move, "Black pawn should be able to go E7->E6");
}

MU_TEST(test_black_pawn_move2turn1) {
    board[tile_name_to_index("E7")] = piece_new(BLACK, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E7")], 
            tile_name_to_index("E5"), 
            board
        );
    mu_assert(can_move, "Black pawn should be able to go E7->E5");
}

MU_TEST(test_white_pawn_attack) {
    board[tile_name_to_index("E5")] = piece_new(WHITE, PAWN);
    board[tile_name_to_index("F6")] = piece_new(BLACK, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E5")], 
            tile_name_to_index("F6"),
            board
        );
    mu_assert(can_move, "White pawn on E5 should be able to attack black pawn on F6");
}

MU_TEST(test_black_pawn_attack) {
    board[tile_name_to_index("E5")] = piece_new(BLACK, PAWN);
    board[tile_name_to_index("D4")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("E5")], 
            tile_name_to_index("D4"),
            board
        );
    mu_assert(can_move, "Black pawn on E5 should be able to attack white pawn on D4");
}

MU_TEST(test_knight_move) {
    board[tile_name_to_index("G1")] = piece_new(WHITE, KNIGHT);
    int can_move = piece_can_move(
            &board[tile_name_to_index("G1")],
            tile_name_to_index("F3"),
            board
            );
    mu_assert(can_move, "Knight should be able to move G1->F3");
}

MU_TEST(test_knight_move_over_pieces) {
    board[tile_name_to_index("G1")] = piece_new(WHITE, KNIGHT);
    board[tile_name_to_index("G2")] = piece_new(BLACK, QUEEN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("G1")],
            tile_name_to_index("F3"),
            board
            );
    mu_assert(can_move, "Knight should be able to move G1->F3 over black queen on G2");
}

MU_TEST(test_knight_move_over_pieces_attacking) {
    board[tile_name_to_index("G1")] = piece_new(WHITE, KNIGHT);
    board[tile_name_to_index("G2")] = piece_new(BLACK, QUEEN);
    board[tile_name_to_index("F3")] = piece_new(BLACK, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("G1")],
            tile_name_to_index("F3"),
            board
            );
    mu_assert(can_move, "Knight should be able to move G1->F3 taking the black pawn on F3, over the black queen on G2");
}

MU_TEST(test_knight_move_over_pieces_attacking_wrong_color) {
    board[tile_name_to_index("G1")] = piece_new(WHITE, KNIGHT);
    board[tile_name_to_index("G2")] = piece_new(BLACK, QUEEN);
    board[tile_name_to_index("F3")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
            &board[tile_name_to_index("G1")],
            tile_name_to_index("F3"),
            board
            );
    mu_assert(!can_move, "Knight should NOT be able to move G1->F3 taking the white pawn on F3, over the black queen on G2");
}

// moving in first quadrant
MU_TEST(bishop_can_move_diagonal_q1) {
    board[tile_name_to_index("E2")] = piece_new(WHITE, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("E2")], 
        tile_name_to_index("G4"), 
        board
    );
    mu_assert(can_move, "Bishop should be able to move E2->G4");
}

MU_TEST(bishop_can_move_diagonal_q2) {
    board[tile_name_to_index("C5")] = piece_new(WHITE, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("C5")], 
        tile_name_to_index("A7"), 
        board
    );
    mu_assert(can_move, "Bishop should be able to move C5->A7");
}

MU_TEST(bishop_can_move_diagonal_q3) {
    board[tile_name_to_index("D5")] = piece_new(WHITE, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("D5")], 
        tile_name_to_index("A2"), 
        board
    );
    mu_assert(can_move, "Bishop should be able to move D5->A2");
}

MU_TEST(bishop_can_move_diagonal_q4) {
    board[tile_name_to_index("D5")] = piece_new(WHITE, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("D5")], 
        tile_name_to_index("F3"), 
        board
    );
    mu_assert(can_move, "Bishop should be able to move D5->F3");
}

MU_TEST(bishop_cant_move_nondiagonal) {
    board[tile_name_to_index("D5")] = piece_new(WHITE, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("D5")], 
        tile_name_to_index("F4"), 
        board
    );
    mu_assert(!can_move, "Bishop should NOT be able to move D5->F4");
}

MU_TEST(bishop_cant_move_over_piece) {
    board[tile_name_to_index("D5")] = piece_new(WHITE, BISHOP);
    board[tile_name_to_index("E4")] = piece_new(BLACK, KNIGHT);
    int can_move = piece_can_move(
        &board[tile_name_to_index("D5")], 
        tile_name_to_index("F3"), 
        board
    );
    mu_assert(!can_move, "Bishop should NOT be able to move D5->F3 with a knight on E4");
}

MU_TEST(bishop_cant_move_over_piece_attacking) {
    board[tile_name_to_index("A1")] = piece_new(BLACK, BISHOP);
    board[tile_name_to_index("H8")] = piece_new(WHITE, QUEEN);
    board[tile_name_to_index("D4")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
        &board[tile_name_to_index("A1")], 
        tile_name_to_index("H8"), 
        board
    );
    mu_assert(!can_move, "Bishop should NOT be able to to move A1 -> H8 with a pawn on D4");
}

MU_TEST(bishop_can_move_taking_opposite_color) {
    board[tile_name_to_index("A1")] = piece_new(BLACK, BISHOP);
    board[tile_name_to_index("H8")] = piece_new(WHITE, QUEEN);
    int can_move = piece_can_move(
        &board[tile_name_to_index("A1")], 
        tile_name_to_index("H8"), 
        board
    );
    mu_assert(!can_move, "Bishop should be able to to move A1 -> H8");
}

MU_TEST(bishop_cant_move_to_own_square) {
    board[tile_name_to_index("A1")] = piece_new(BLACK, BISHOP);
    int can_move = piece_can_move(
        &board[tile_name_to_index("A1")],
        tile_name_to_index("A1"), 
        board
    );
    mu_assert(!can_move, "Bishop should not be able to move A1->A1");
}

MU_TEST(rook_can_move_horizontal) {
    board[tile_name_to_index("B2")] = piece_new(WHITE, ROOK);
    int can_move = piece_can_move(
        &board[tile_name_to_index("B2")], 
        tile_name_to_index("G2"),
        board
    );
    mu_assert(can_move, "Rook should be able to move B2->G2");
}

MU_TEST(rook_can_move_vertical) {
    board[tile_name_to_index("B2")] = piece_new(WHITE, ROOK);
    int can_move = piece_can_move(
        &board[tile_name_to_index("B2")], 
        tile_name_to_index("B6"),
        board
    );
    mu_assert(can_move, "Rook should be able to move B2->B6");
}

MU_TEST(rook_cant_move_diagonal) {
    board[tile_name_to_index("B2")] = piece_new(WHITE, ROOK);
    int can_move = piece_can_move(
        &board[tile_name_to_index("B2")], 
        tile_name_to_index("D4"),
        board
    );
    mu_assert(!can_move, "Rook should NOT be able to move B2->D4");
}

MU_TEST(rook_cant_move_over_piece_vertical) {
    board[tile_name_to_index("B2")] = piece_new(WHITE, ROOK);
    board[tile_name_to_index("B4")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
        &board[tile_name_to_index("B2")], 
        tile_name_to_index("B6"),
        board
    );
    mu_assert(!can_move, "Rook should NOT be able to move B2->B6 with a pawn on B4");
}

MU_TEST(rook_cant_move_over_piece_horizontal) {
    board[tile_name_to_index("B2")] = piece_new(WHITE, ROOK);
    board[tile_name_to_index("G2")] = piece_new(WHITE, PAWN);
    int can_move = piece_can_move(
        &board[tile_name_to_index("B2")], 
        tile_name_to_index("H2"),
        board
    );
    mu_assert(!can_move, "Rook should NOT be able to move B2->H2 with a pawn on G2");
}

MU_TEST(queen_can_move_diagonal_q3) {
    board[tile_name_to_index("D5")] = piece_new(WHITE, QUEEN);
    int can_move = piece_can_move(
        &board[tile_name_to_index("D5")], 
        tile_name_to_index("A2"), 
        board
    );
    mu_assert(can_move, "Queen should be able to move D5->A2");
}

MU_TEST(queen_can_move_doesnt_change_board) {
    board[tile_name_to_index("A2")] = piece_new(WHITE, QUEEN);
    piece_can_move(
        &board[tile_name_to_index("A2")], 
        tile_name_to_index("A4"), 
        board
    );
    mu_assert(board[tile_name_to_index("A2")]->kind == QUEEN, "Testing if a queen can move should not change the value on the queen's square");
}

MU_TEST(format_raw_input_trims_whitespace) {
    char *format = format_raw_input(" \n  17 \n  ");
    mu_assert_string_eq("17", format);
}

MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_tile_name_to_indexA1);
    MU_RUN_TEST(test_tile_name_to_indexE4);
    MU_RUN_TEST(test_index_to_tile_nameD2);
    MU_RUN_TEST(test_get_by_row_col);

    MU_RUN_TEST(test_white_pawn_move1turn1);
    MU_RUN_TEST(test_white_pawn_move2turn1);
    MU_RUN_TEST(test_black_pawn_move1turn1);
    MU_RUN_TEST(test_black_pawn_move2turn1);
    MU_RUN_TEST(test_white_pawn_attack);
    MU_RUN_TEST(test_black_pawn_attack);

    MU_RUN_TEST(test_knight_move);
    MU_RUN_TEST(test_knight_move_over_pieces);
    MU_RUN_TEST(test_knight_move_over_pieces_attacking);
    MU_RUN_TEST(test_knight_move_over_pieces_attacking_wrong_color);

    MU_RUN_TEST(bishop_can_move_diagonal_q1);
    MU_RUN_TEST(bishop_can_move_diagonal_q2);
    MU_RUN_TEST(bishop_can_move_diagonal_q3);
    MU_RUN_TEST(bishop_can_move_diagonal_q4);
    MU_RUN_TEST(bishop_cant_move_nondiagonal);
    MU_RUN_TEST(bishop_cant_move_over_piece);
    MU_RUN_TEST(bishop_cant_move_over_piece_attacking);
    MU_RUN_TEST(bishop_can_move_taking_opposite_color);
    MU_RUN_TEST(bishop_cant_move_to_own_square);

    MU_RUN_TEST(rook_can_move_horizontal);
    MU_RUN_TEST(rook_can_move_vertical);
    MU_RUN_TEST(rook_cant_move_diagonal);
    MU_RUN_TEST(rook_cant_move_over_piece_vertical);
    MU_RUN_TEST(rook_cant_move_over_piece_horizontal);

    MU_RUN_TEST(queen_can_move_diagonal_q3);
    MU_RUN_TEST(queen_can_move_doesnt_change_board);

    MU_RUN_TEST(format_raw_input_trims_whitespace);
}


int main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}

