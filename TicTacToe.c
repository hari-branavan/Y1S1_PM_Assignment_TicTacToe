#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game_init.h"

//Main function

int main(){
    // declaring required variables
    int n, mode, mRow, mColumn, inputResult;
    char sign = 'X';
    bool gameOver = false;
    Player players[3];
    
    // generating seed value based on time
    srand(time(NULL));
    
    // opening gameState file for writing
    FILE *gameState = fopen("GameState.txt", "w");

    // error handling for file
    if (gameState == NULL) {
        printf("Unable to open file for writing\n");
        return 1;
    }
    
    // gives game instructions
    helper();

    // getting user input on grid size
    printf("Enter the grid size (3 <= N <= 10): ");
    int sizeResult = scanf("%d", &n);
    
    // input handling for grid size
    while (n < 3 || n > 10 || sizeResult != 1) {
        printf("\nInvalid size input\n\n");

        char buff1 = getchar();
        while (buff1 != '\n') {
            buff1 = getchar();
        }

        printf("Enter the grid size (3 <= N <= 10): ");
        sizeResult = scanf("%d", &n);
    }

    // pointer to pointer board variable calls makeBoard function. 
    // makeBoard returns the first row first column element to this variable
    char **board = makeBoard(n);

    // user input for game mode selection
    printf("\nSelect game mode: \n1. Two Players(User vs User)\n2. User vs Computer\n3. Three players\nEnter choice (1-3): ");
    int modeResult = scanf("%d", &mode);
    
    // input handling for game mode input
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
    
    // displaying empty board for user
    printf("\n");
    displayBoard(board, n);
    
    // instructions to follow until gameOver variable becomes true
    while (!gameOver){
        gameOver = playTurn(gameState, n, mode, mRow, mColumn, sign, inputResult, board, players);
        
        sign = switchSign(sign, mode);
        displayBoard(board, n);
    }

    // freeing dynamically allocated memory and closing the IO file.
    freeBoard(board, n);
    fclose(gameState);

    // returning 0 to show program ran successfully
    return 0;
}

