#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// using custom header file consisting of function prototypes and structure definitions
#include "game_init.h"

// function to make computer detect if it can win in one move
Move tryComputerWin(char **board, char sign, int n){
    // default coords of type Move structure to return if no win move was found
    Move coords = {-1, -1};

    // Horizontal Check
    for (int row = 0; row < n; row++){
        for (int col = 0; col < 1; col++){
            int sameCount = 0;
            int emptyPos = -1;
            // third for loop to access each column while in the same row(second FOR loop)
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

            // third for loop to access each row while in the same column(second FOR loop)
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

    // Diagonal Check
    int sameCount = 0;
    int emptyPos = -1;
    
    // no need for row and column FOR loop because row = column so incrementing k is enough
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

    // Anti-diagonal Check
    sameCount = 0;
    emptyPos = -1;
    
    // no need for row and column FOR loop because row = column so incrementing k is enough
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

    return coords; // no winning move found, returning {-1, -1}
}

// function to check if computer can block opponents move if they can win in one move
Move tryComputerBlock(char **board, char sign, int n, int mode){
    // declaring default coords of type Move Structure to return if no block move was found
    Move coords = {-1, -1};

    // for both mode, these are all the possible opponents
    char possibleOpponents[3];
    int numOpponents = 0;

    // depending on the mode, possibleOpponents array will be assigned sign characters
    // number of opponents will also be assigned
    if (mode == 3) {
        possibleOpponents[0] = 'X';
        possibleOpponents[1] = 'O';
        possibleOpponents[2] = 'Z';

        numOpponents = 3;
    } else {
        possibleOpponents[0] = 'X';
        possibleOpponents[1] = 'O';

        numOpponents = 2;
    }

    // looping for each opponent and skipping loop if it accesses the players index
    for (int opIndex = 0; opIndex < numOpponents; opIndex++){
        char opponent = possibleOpponents[opIndex];

        if (opponent == sign){
            continue;
        }

        // Horizontal Check
        for (int row = 0; row < n; row++){
            for (int col = 0; col < 1; col++) {
                int oppCount = 0; // to store the number of opponent characters in a row
                int emptyPos = -1; // to store which row or column number to play in order to block opponent

                // to access each column while in same row
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

                // checking if opponent is about to win and a possible block move exists
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

                // accessing each row while in the same column
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

        // diagonal check
        int oppCount = 0;
        int emptyPos = -1;

        // row = column so one FOR loop is enough
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

        // anti-diagonal check (top-right to bottom-left)
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

void computerMove(FILE *gameState, char **board, int n, char sign, int mode){
    int row, col; // declaring row and column variables

    // variable co_ords of type Move to store returned coordinates from tryComputerWin function
    Move co_ords = tryComputerWin(board, sign, n);

    if (co_ords.row == -1 && co_ords.col == -1){
        // try blocking move if win is not possible
        co_ords = tryComputerBlock(board, sign, n, mode);
    }

    // if both win and block move is not possible, do a random move on board
    if (co_ords.row == -1 && co_ords.col == -1){
        do{
            row = rand() % n;
            col = rand() % n;
        } while (*(*(board + row) + col) != '-');

    } else {
        row = co_ords.row;
        col = co_ords.col;
    }

    // assign move to specific row and column in board using pointer arithmetic
    *(*(board + row) + col) = sign;
    // write computer move to GameState.txt
    fprintf(gameState, "Computer made a move at row - %d, column - %d.", row+1, col+1);
}

