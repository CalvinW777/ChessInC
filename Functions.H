#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

bool inBounds(int r, int c);
void clearMoves(bool moves[8][8]);

void generateMoves(bool moves[8][8], int board[8][8], int row, int col);

bool findKing(int board[8][8], bool white, int *kingRow, int *kingCol);
bool squareAttacked(int board[8][8], int row, int col, bool byWhite);
bool moveLeavesKingInCheck(int board[8][8], int sr, int sc, int tr, int tc);
bool hasLegalMove(int board[8][8], bool whiteTurn);

extern bool whiteKingMoved;
extern bool blackKingMoved;

extern bool whiteRookLeftMoved;
extern bool whiteRookRightMoved;

extern bool blackRookLeftMoved;
extern bool blackRookRightMoved;

extern int enPassantRow;
extern int enPassantCol;

#endif