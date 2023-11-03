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


MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_tile_name_to_indexA1);
    MU_RUN_TEST(test_tile_name_to_indexE4);
    MU_RUN_TEST(test_index_to_tile_nameD2);

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
}


int main(void) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}

