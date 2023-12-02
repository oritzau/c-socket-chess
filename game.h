// here to (hopefully) reduce the amount of magic numbers
#define ROW_SIZE 8
#define COL_SIZE 8 
#define BOARD_SIZE ROW_SIZE * COL_SIZE
#define NULL_PIECE_ERROR -1

typedef enum PieceKind {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
} PieceKind;

typedef enum PieceColor {
    WHITE = 0,
    BLACK = 1,
} PieceColor;

typedef struct Piece {
    PieceColor color;
    PieceKind kind;
    int has_moved;
} Piece;

// board is just 64 Piece pointers in an array
typedef struct Piece ** Board; 

typedef struct Move {
    int active_index;
    int target_index;
    Board board;
} Move;

Board board_new();
Piece * piece_new(PieceColor color, PieceKind kind);
Move move_new(Piece * piece, unsigned char target_index, Board board);
Piece piece_copy(Piece * piece);

void board_print(Board board);
Piece * board_index_by_row_col(Board board, int row, int col);
char * tile_index_to_name(int index);
int tile_name_to_index(char * tile_name);
int get_piece_index(Piece ** piece, Board board);

int piece_can_move(Piece ** piece, int target_index, Board board);

int move_is_valid(Move *move);

char * format_raw_input(char *input);

Move * parse_player_intent(char *formatted_input);
