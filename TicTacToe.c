#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game_init.h"

//Main function

int main(){
    int n, mode, mRow, mColumn, inputResult;
    char sign = 'X';
    bool gameOver = false;
    Player players[3];

    srand(time(NULL));

    FILE *gameState = fopen("GameState.txt", "w");

    if (gameState == NULL) {
        printf("Unable to open file for writing\n");
        return 1;
    }
    
    printf("Enter the grid size (3 <= N <= 10): ");
    int sizeResult = scanf("%d", &n);
    
    while (n < 3 || n > 10 || sizeResult != 1) {
        printf("\nInvalid size input\n\n");

        char buff1 = getchar();
        while (buff1 != '\n') {
            buff1 = getchar();
        }

        printf("Enter the grid size (3 <= N <= 10): ");
        sizeResult = scanf("%d", &n);
    }

    char **board = createBoard(n);

    printf("\nSelect game mode: \n1. Two Players(User vs User)\n2. User vs Computer\n3. Three players\nEnter choice (1-3): ");
    int modeResult = scanf("%d", &mode);

    while (mode < 1 || mode > 3 || modeResult != 1){
        printf("\nInvalid mode selected\n");

        char buff2 = getchar();
        while (buff2 != '\n'){
            buff2 = getchar();
        }

        printf("\nThere are 3 game modes\n1. User vs User\n2. User vs Computer\n3. 3 Users\n\nChoose a game mode: ");
        modeResult = scanf("%d", &mode);
    }

    if (mode == 3){
        ThreePlayerConfig(players);
    }
    
    printf("\n");
    displayBoard(board, n);
    
    while (!gameOver){
        gameOver = playTurn(gameState, n, mode, mRow, mColumn, sign, inputResult, board, players);
        
        sign = switchSign(sign, mode);
        displayBoard(board, n);
    }

    freeBoard(board, n);
    fclose(gameState);
    return 0;
}

