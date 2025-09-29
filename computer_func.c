#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game_init.h"

Move tryComputerWin(char **board, char sign, int n){
    Move coords = {-1, -1};

    // Horizontal Check
    for (int row = 0; row < n; row++){
        for (int col = 0; col < 1; col++){
            int sameCount = 0;
            int emptyPos = -1;

            for (int k = 0; k < n; k++){
                if (board[row][col+k] == sign){
                    sameCount ++;
                } else if (board[row][col+k] == '-'){
                    if (emptyPos == -1){
                        emptyPos = k;
                    } else {
                        break;
                    }
                }
            }

            if (sameCount == n-1 && emptyPos != -1){
                coords.row = row;
                coords.col = col + emptyPos;
                return coords;
            }
        }
    }

    // Vertical Check
    for (int row = 0; row < 1; row++){
        for (int col = 0; col < n; col++){
            int sameCount = 0;
            int emptyPos = -1;

            for (int k = 0; k < n; k++){
                if (board[row+k][col] == sign){
                    sameCount++;
                } else if (board[row+k][col] == '-'){
                    if (emptyPos == -1){
                        emptyPos = k;
                    } else {
                        break;
                    }
                }
            }

            if (sameCount == n-1 && emptyPos != -1){
                coords.row = row + emptyPos;
                coords.col = col;
                return coords;
            }
        }
    }

    // Diagonal Check (top-left to bottom-right)
    int sameCount = 0;
    int emptyPos = -1;

    for (int k = 0; k < n; k++){
        if (board[k][k] == sign){
            sameCount++;
        } else if (board[k][k] == '-'){
            if (emptyPos == -1){
                emptyPos = k;
            } else {
                emptyPos = -1;
                break;
            }
        }
    }

    if (sameCount == n-1 && emptyPos != -1){
        coords.row = emptyPos;
        coords.col = emptyPos;
        return coords;
    }

    // Anti-diagonal Check (top-right to bottom-left)
    sameCount = 0;
    emptyPos = -1;

    for (int k = 0; k < n; k++){
        if (board[k][n - 1 - k] == sign){
            sameCount++;
        } else if (board[k][n - 1 - k] == '-'){
            if (emptyPos == -1){
                emptyPos = k;
            } else {
                emptyPos = -1;
                break;
            }
        }
    }

    if (sameCount == n-1 && emptyPos != -1){
        coords.row = emptyPos;
        coords.col = n - 1 - emptyPos;
        return coords;
    }

    return coords; // no winning move found
}

Move tryComputerBlock(char **board, char sign, int n){
    Move coords = {-1, -1};

    char possibleOpponents[] = {'X','O','Z'};

    for (int opIndex = 0; opIndex < 3; opIndex++){
        char opponent = possibleOpponents[opIndex];

        if (opponent == sign){
            continue;
        }

        bool opponentExists = false;
        for (int i = 0; i < n && !opponentExists; i++){
            for (int j = 0; j < n && !opponentExists; j++){
                if (board[i][j] == opponent){
                    opponentExists = true;
                }
            }
        }

        if (!opponentExists){
            continue;
        }

        // Horizontal Check
        for (int row = 0; row < n; row++){
            for (int col = 0; col < 1; col++){
                int oppCount = 0;
                int emptyPos = -1;

                for (int k = 0; k < n; k++){
                    if (board[row][col + k] == opponent){
                        oppCount++;
                    } else if (board[row][col + k] == '-'){
                        if (emptyPos == -1){
                            emptyPos = k;
                        } else {
                            break;
                        }
                    }
                }

                if (oppCount == (n-1) && emptyPos != -1){
                    coords.row = row;
                    coords.col = col + emptyPos;
                    return coords;
                }
            }
        }

        // Vertical Check
        for (int row = 0; row < 1; row++){
            for (int col = 0; col < n; col++){
                int oppCount = 0;
                int emptyPos = -1;

                for (int k = 0; k < n; k++){
                    if (board[row + k][col] == opponent){
                        oppCount++;
                    } else if (board[row + k][col] == '-'){
                        if (emptyPos == -1){
                            emptyPos = k;
                        } else {
                            break;
                        }
                    }
                }

                if (oppCount == (n-1) && emptyPos != -1){
                    coords.row = row + emptyPos;
                    coords.col = col;
                    return coords;
                }
            }
        }

        // Diagonal Check (top-left to bottom-right)
        int oppCount = 0;
        int emptyPos = -1;

        for (int k = 0; k < n; k++){
            if (board[k][k] == opponent){
                oppCount++;
            } else if (board[k][k] == '-'){
                if (emptyPos == -1){
                    emptyPos = k;
                } else {
                    emptyPos = -1;
                    break;
                }
            }
        }

        if (oppCount == (n-1) && emptyPos != -1){
            coords.row = emptyPos;
            coords.col = emptyPos;
            return coords;
        }

        // Diagonal Check (top-right to bottom-left)
        oppCount = 0;
        emptyPos = -1;

        for (int k = 0; k < n; k++){
            if (board[k][n - 1 - k] == opponent){
                oppCount++;
            } else if (board[k][n - 1 - k] == '-'){
                if (emptyPos == -1){
                    emptyPos = k;
                } else {
                    emptyPos = -1;
                    break;
                }
            }
        }

        if (oppCount == (n-1) && emptyPos != -1){
            coords.row = emptyPos;
            coords.col = n - 1 - emptyPos;
            return coords;
        }
    }

    return coords; // no block found
}

void computerMove(FILE *gameState, char **board, int n, char sign){
    int row, col;

    Move co_ords = tryComputerWin(board, sign, n);

    if (co_ords.row == -1 && co_ords.col == -1){
        co_ords = tryComputerBlock(board, sign, n);
    }

    if (co_ords.row == -1 && co_ords.col == -1){
        do{
            row = rand() % n;
            col = rand() % n;
        } while (*(*(board + row) + col) != '-');

    } else {
        row = co_ords.row;
        col = co_ords.col;
    }

    *(*(board + row) + col) = sign;
    fprintf(gameState, "Computer made a move at row - %d, column - %d.", row+1, col+1);
}

