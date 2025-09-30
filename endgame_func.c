#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game_init.h"

bool checkWin(char **board, int n, char sign){
    // Diagonal check
    bool win = true;

    for (int k = 0; k < n; k++) {
        if (board[k][k] != sign) {
            win = false;
            break;
        }
    }
    if (win) return true;

    // Anti-diagonal check
    win = true;

    for (int k = 0; k < n; k++) {
        if (board[k][n-1-k] != sign) {
            win = false;
            break;
        }
    }
    if (win) return true;

    // Horizontal check
    for (int row = 0; row < n; row++){
        for (int col = 0; col < 1; col++){
            bool win = true;

            for (int k = 0; k < n; k++){
                if (board[row][col+k] != sign){
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    // Vertical check
    for (int row = 0; row < 1; row++){
        for (int col = 0; col < n; col++){
            bool win = true;

            for (int k = 0; k < n; k++) {
                if (board[row + k][col] != sign) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    return false;
}

bool checkDraw(char **board, int n, int mode){
    char players[3];
    int playerNum;

    if (mode == 3) {
        players[0] = 'X';
        players[1] = 'O';
        players[2] = 'Z';
        playerNum = 3;
    } else {
        players[0] = 'X';
        players[1] = 'O';
        playerNum = 2;
    }

    for (int p = 0; p < playerNum; p++){
        char nowPlayer = players[p];

        // checking rows
        for (int row = 0; row < n; row++){
            bool rowWinPossible = true;
            for (int col = 0; col < 1; col++){
                for (int k = 0; k < n; k++){
                    if (board[row][col + k] != '-' && board[row][col + k] != nowPlayer){
                        rowWinPossible = false;
                        break;
                    }
                }
            }
            if (rowWinPossible) return false; // because game can continue
        }

        for (int row = 0; row < 1; row++){
            for (int col = 0; col < n; col++){
                bool colWinPossible = true;
                for (int k = 0; k < n; k++){
                    if (board[row + k][col] != '-' && board[row + k][col] != nowPlayer){
                        colWinPossible = false;
                        break;
                    }
                }
                if (colWinPossible) return false; // because game can continue
            }
        }

        bool diagWinPossible = true;

        for (int k = 0; k < n; k++){
            if (board[k][k] != '-' && board[k][k] != nowPlayer){
                diagWinPossible = false;
                break;
            }
        }

        if (diagWinPossible) return false;

        diagWinPossible = true;

        for (int k = 0; k < n; k++){
            if (board[k][n - 1 - k] != '-' && board[k][n -1 - k] != nowPlayer){
                diagWinPossible = false;
                break;
            }
        }

        if (diagWinPossible) return false;
    }

    return true;
}
