#include <stdbool.h>
#ifndef struct_h
#define struct_h

struct Piece{
        int color;// 0 = white, 1 = black
        int pieceType;// 0 = pawn, 1 = rook, 2 = bishop, 3 = knight, 4 = queen, 5 = king
        bool firstMove;
        int moveCount;
    };
    struct Square {
        struct Piece* currPiece;
        int col;
        int row;
    };
    struct Move{
        int initialCol;
        int initialRow;
        int destinationCol;
        int destinationRow;

    };
#endif 
