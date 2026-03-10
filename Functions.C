#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Functions.h"

int rookDirs[4][2] = {
    {1,0}, {-1,0}, {0,1}, {0,-1}
};
int bishopDirs[4][2] = {
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
};
int queenDirs[8][2] = {
    {1,0}, {-1,0}, {0,1}, {0,-1},
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
};

bool whiteKingMoved = false;
bool blackKingMoved = false;

bool whiteRookLeftMoved = false;
bool whiteRookRightMoved = false;

bool blackRookLeftMoved = false;
bool blackRookRightMoved = false;

int enPassantRow = -1;
int enPassantCol = -1;

bool inBounds(int r, int c)//checks if a square is within the bounds of the board, used for move generation and validation
{
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

void clearMoves(bool moves[8][8])//clears the moves array, used before generating moves for a piece
{
    for (int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            moves[i][j] = false;

}

void generateKniteMoves(bool moves[8][8], int board[8][8], int row, int col)//generates moves for knights, checks all 8 possible moves and adds them if valid
{
  int offset[8][2] = {
    {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
    {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
  };
  
  int piece = board[row][col];

  for(int i = 0; i < 8; i++){
    int r = row + offset[i][0];
    int c = col + offset[i][1];
    
    if(inBounds(r,c))
        if(board[r][c] == 0 || board[r][c] * piece < 0)
            moves[r][c] = true;
  }
}

void generatePawnMoves(bool moves[8][8], int board[8][8], int row, int col){//generates moves for pawns, also checks for double move and captures
    int piece = board[row][col];
    int dir;

    if(piece > 0)
        dir = -1;
    else
        dir = 1;

    int r = row + dir;

    //move forward
    if(inBounds(r,col) && board[r][col] == 0)
        moves[r][col] = true;
    
    //double move
    if(piece > 0 && row == 6 && board[row-1][col] == 0 && board[row-2][col] == 0)
        moves[row-2][col] = true;
    else if(piece < 0 && row == 1 && board[row+1][col] == 0 && board[row+2][col] == 0)
        moves[row+2][col] = true;

    //Take Left
    int c = col - 1;
    if(inBounds(r,c) && board[r][c] * piece < 0)
        moves[r][c] = true;
    
    //Take Right
    c = col + 1;
    if(inBounds(r,c) && board[r][c] * piece < 0)
        moves[r][c] = true;
    
    // EN PASSANT

    if(piece > 0) // white pawn
    {
        if(row == 3 && abs(col - enPassantCol) == 1 && enPassantRow == 2)
        {
            if(board[3][enPassantCol] == -1)   // enemy pawn beside
                moves[2][enPassantCol] = true;
        }
    }

    if(piece < 0) // black pawn
    {
        if(row == 4 && abs(col - enPassantCol) == 1 && enPassantRow == 5)
        {
            if(board[4][enPassantCol] == 1)    // enemy pawn beside
                moves[5][enPassantCol] = true;
        }
    }


}

void generateSlidingMoves(bool moves[8][8], int board[8][8], int row, int col, int dirs[][2], int dirCount)//generates moves for rooks, bishops, and queens based on the provided directions
{
    int piece = board[row][col];

    for(int d = 0; d < dirCount; d++)
    {
        int r = row;
        int c = col;

        while(true)
        {
            r += dirs[d][0];
            c += dirs[d][1];

            if(!inBounds(r,c))
                break;

            if(board[r][c] == 0)
            {
                moves[r][c] = true;
            }
            else
            {
                if(board[r][c] * piece < 0)
                    moves[r][c] = true;

                break;
            }
        }
    }
}
void generateKingMoves(bool moves[8][8], int board[8][8], int row, int col)//generates moves for the king, also checks for castling rights and adds those moves if applicable
{
    int offsets[8][2] = {
        {1,0},{-1,0},{0,1},{0,-1},
        {1,1},{1,-1},{-1,1},{-1,-1}
    };

    int piece = board[row][col];

    for(int i=0;i<8;i++)
    {
        int r = row + offsets[i][0];
        int c = col + offsets[i][1];

        if(inBounds(r,c))
        {
            if(board[r][c] == 0 || board[r][c]*piece < 0)
                moves[r][c] = true;
        }
    }

    if(piece > 0 && !whiteKingMoved)
    {
        // kingside
        if(!whiteRookRightMoved &&
        board[7][5] == 0 &&
        board[7][6] == 0)
            moves[7][6] = true;

        // queenside
        if(!whiteRookLeftMoved &&
        board[7][1] == 0 &&
        board[7][2] == 0 &&
        board[7][3] == 0)
            moves[7][2] = true;
    }
    if(piece < 0 && !blackKingMoved)
    {
    // kingside
    if(!blackRookRightMoved &&
       board[0][5] == 0 &&
       board[0][6] == 0)
        moves[0][6] = true;

    // queenside
    if(!blackRookLeftMoved &&
       board[0][1] == 0 &&
       board[0][2] == 0 &&
       board[0][3] == 0)
        moves[0][2] = true;
    }
}

void generateMoves(bool moves[8][8], int board[8][8], int row, int col)//generates moves for a piece, calls the appropriate function based on piece type
{
    int piece = abs(board[row][col]);

    switch(piece)
    {
        case 1:
            generatePawnMoves(moves, board, row, col);
            break;

        case 2:
            generateSlidingMoves(moves, board, row, col, rookDirs, 4);
            break;

        case 3:
            generateKniteMoves(moves, board, row, col);
            break;

        case 4:
            generateSlidingMoves(moves, board, row, col, bishopDirs, 4);
            break;

        case 5:
            generateSlidingMoves(moves, board, row, col, queenDirs, 8);
            break;

        case 6:
            generateKingMoves(moves, board, row, col);
            break;
    }
}

bool findKing(int board[8][8], bool white, int* kingRow, int* kingCol){//finds the king of the specified color, used for check detection and move validation
    int king = white ? 6 : -6;
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            if(board[r][c] == king){
                *kingRow = r;
                *kingCol = c;
                return true;

            }
        }
    }
    return false;
}

bool squareAttacked(int board[8][8], int row, int col, bool byWhite){//checks if a square is attacked by the opponent, used for check detection and move validation
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            int piece = board[r][c];
            if(piece != 0 && ((byWhite && piece > 0) || (!byWhite && piece < 0))){
                bool moves[8][8];
                clearMoves(moves);
                generateMoves(moves, board, r, c);
                if(moves[row][col])
                    return true;
            }
        }
    }
    return false;
}

bool moveLeavesKingInCheck(int board[8][8], int sr, int sc, int tr, int tc){//checks if a move you request leaves your king in check, used for move validation
    int tempBoard[8][8];

    for(int r=0;r<8;r++)
        for(int c=0;c<8;c++)
            tempBoard[r][c] = board[r][c];

    int piece = tempBoard[sr][sc];

    tempBoard[tr][tc] = piece;
    tempBoard[sr][sc] = 0;

    bool white = piece > 0;

    int kr, kc;
    findKing(tempBoard, white, &kr, &kc);

    return squareAttacked(tempBoard, kr, kc, !white);
}

bool hasLegalMove(int board[8][8], bool whiteTurn){//checks if the current player has any legal moves, used for checkmate and stalemate detection
    bool moves[8][8];
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            int piece = board[r][c];
            if(piece == 0)
                continue;

            if((whiteTurn && piece > 0) || (!whiteTurn && piece < 0)){
                clearMoves(moves);
                generateMoves(moves, board, r, c);
                
                for(int tr=0; tr<8; tr++){
                    for(int tc=0; tc<8; tc++){
                        if(moves[tr][tc] && !moveLeavesKingInCheck(board, r, c, tr, tc)){
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}