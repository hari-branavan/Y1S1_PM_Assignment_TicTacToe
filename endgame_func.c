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

bool checkDraw(char **board, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (*(*(board+i)+j) == '-'){
                return false;
            }
        }
    }
    return true;
}

