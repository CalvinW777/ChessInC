#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Functions.H"

int main() {//main function, initializes the game and contains the game loop
    InitWindow(1024, 1024, "C Chess Project");
    printf("Working directory: %s\n", GetWorkingDirectory());
    SetTargetFPS(60);
    SetWindowMinSize(1024, 1024);
    SetWindowSize(1024, 1024);
    
    int board[8][8] = {
        {-2,-3,-4,-5,-6,-4,-3,-2},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        { 1, 1, 1, 1, 1, 1, 1, 1},
        { 2, 3, 4, 5, 6, 4, 3, 2}
    };
    
    Texture2D pieces[12];
    pieces[0]  = LoadTexture("assets/white-pawn.png");
    pieces[1]  = LoadTexture("assets/white-rook.png");
    pieces[2]  = LoadTexture("assets/white-knight.png");
    pieces[3]  = LoadTexture("assets/white-bishop.png");
    pieces[4]  = LoadTexture("assets/white-queen.png");
    pieces[5]  = LoadTexture("assets/white-king.png");

    pieces[6]  = LoadTexture("assets/black-pawn.png");
    pieces[7]  = LoadTexture("assets/black-rook.png");
    pieces[8]  = LoadTexture("assets/black-knight.png");
    pieces[9]  = LoadTexture("assets/black-bishop.png");
    pieces[10] = LoadTexture("assets/black-queen.png");
    pieces[11] = LoadTexture("assets/black-king.png");

    int selectedRow = -1;
    int selectedCol = -1;

    bool legalMoves[8][8] = {0};
    bool turnControl = true; //true = white's turn, false = black

    printf("pawn texture width: %d\n", pieces[0].width);
 
    while(!WindowShouldClose()){
        int tileSize = (GetScreenWidth() < GetScreenHeight() ? GetScreenWidth() : GetScreenHeight()) / 8;

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                int col = GetMouseX() / tileSize;
                int row = GetMouseY() / tileSize;  

                if(inBounds(row, col)){
                    // move piece
                    if(legalMoves[row][col] && !moveLeavesKingInCheck(board, selectedRow, selectedCol, row, col)){
                        int piece = board[selectedRow][selectedCol];

                        // en pessant capture
                        if(piece == 1 && selectedRow == 3 && row == enPassantRow && col == enPassantCol){
                            board[row + 1][col] = 0;
                        }

                        if(piece == -1 && selectedRow == 4 && row == enPassantRow && col == enPassantCol){
                            board[row - 1][col] = 0;
                        }

                        // move piece
                        board[row][col] = piece;
                        board[selectedRow][selectedCol] = 0;

                        // reset en passant
                        enPassantRow = -1;
                        enPassantCol = -1;

                        // detect double pawn move
                        if(piece == 1 && selectedRow == 6 && row == 4){
                            enPassantRow = 5;
                            enPassantCol = col;
                        }

                        if(piece == -1 && selectedRow == 1 && row == 3){
                            enPassantRow = 2;
                            enPassantCol = col;
                        }

                        if(abs(piece) == 6){//castling, moves rook if king moves 2 spaces
                            if(selectedCol == 4 && col == 6){//kingside
                                board[row][5] = board[row][7];
                                board[row][7] = 0;
                            }
                            if(selectedCol == 4 && col == 2){//queenside
                                board[row][3] = board[row][0];
                                board[row][0] = 0;
                            }
                        }

                        board[row][col] = piece;
                        board[selectedRow][selectedCol] = 0;

                        if(piece == 1 && row == 0)//white pawn promo 
                            board[row][col] = 5; 

                        if(piece == -1 && row == 7)//black pawn promo
                            board[row][col] = -5;


                        // update castling rights
                        if(piece == 6) whiteKingMoved = true;
                        if(piece == -6) blackKingMoved = true;

                        if(piece == 2 && selectedRow == 7 && selectedCol == 0) whiteRookLeftMoved = true;
                        if(piece == 2 && selectedRow == 7 && selectedCol == 7) whiteRookRightMoved = true;

                        if(piece == -2 && selectedRow == 0 && selectedCol == 0) blackRookLeftMoved = true;
                        if(piece == -2 && selectedRow == 0 && selectedCol == 7) blackRookRightMoved = true;

                        selectedRow = -1;
                        selectedCol = -1;

                        clearMoves(legalMoves);
                        turnControl = !turnControl;

                        int kr, kc;//find king position after move to check for checkmate/stalemate
                        bool white = turnControl;

                        findKing(board, white, &kr, &kc);

                        bool inCheck = squareAttacked(board, kr, kc, !white);
                        bool canMove = hasLegalMove(board, white);

                        if(!canMove)
                        {
                            if(inCheck)
                                printf("CHECKMATE\n");
                            else
                                printf("STALEMATE\n");
                        }
                    }
                    else
                    {
                        int piece = board[row][col];

                        if(piece != 0)
                        {
                            if((turnControl && piece > 0) || (!turnControl && piece < 0))
                            {
                                selectedRow = row;
                                selectedCol = col;

                                clearMoves(legalMoves);
                                generateMoves(legalMoves, board, row, col);
                            }
                            
                        }
                    }
                }
            }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){

                Color squareColor = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKGRAY;

                if(row == selectedRow && col == selectedCol)
                    squareColor = RED;
                
                if(legalMoves[row][col])
                    squareColor = GREEN;

                DrawRectangle(col * tileSize, row * tileSize, tileSize, tileSize, squareColor); 

                int piece = board[row][col];
                if (piece != 0){
                    int index;
                    if (piece > 0){
                        index = piece - 1;
                    } else {
                        index = (-piece - 1) + 6;
                    }
                    DrawTexture(pieces[index], col * tileSize, row * tileSize, WHITE);
                }

            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}



