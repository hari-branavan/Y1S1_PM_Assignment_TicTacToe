#include <stdio.h>
#include <stdbool.h>

#include "game_init.h"

bool playTurn(FILE *gameState, int n, int mode, int mRow, int mColumn, char sign, int inputResult, char **board, Player players[]){
    // variable used in 3 player mode to store the current player
    int currentPlayerIndex = -1;
    
    // using switch on mode variable to determine which game flow to follow
    // User vs User, User vs Computer, 3 Player

    switch (mode){
        case 1:
            // prompt each user to enter move by checking the current sign
            // inputResult is a variable that is passed to the validMove function. To check if both input values are numbers only.
            if (sign == 'X'){
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);
            } else {
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);
            }
            
            // recording which players turn it was in GameState.txt
            fprintf(gameState, "Player %c's turn.\n", sign);

            printf("\n");
            
            // passing the relevant variables to validMove function to check if the input is valid
            validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);
            
            // checking for endgame conditions. If false is returned from both the functions, game continues
            if (checkWin(board, n, sign) == 1){
                printf("Player %c Wins!\n\n", sign);
                fprintf(gameState, "\nPlayer %c won!\n", sign);
                return true;
            } else if (checkDraw(board, n, mode)== 1){
                printf("It's a draw!\n\n");
                fprintf(gameState, "\nIt's a draw!\n");
                return true;
            } else {
                fprintf(gameState, "\nGame Ongoing...");
                fprintf(gameState, "\n--------------------------\n");
            }

            break;

        case 2:
            // prompt each user to enter move by checking the current sign
            if (sign == 'X'){
                printf("\nPlayer %c to move. Please enter coordinate wise - [row][space][column]: ", sign);
                fprintf(gameState, "Player %c's turn.\n", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);
                // passing variables to function to check for input validity
                validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);
            } else {
                printf("\nComputer's turn: \n");
                fprintf(gameState, "Computer's turn.\n");
                computerMove(gameState, board, n, sign, 2);
            }

            printf("\n");

            // checking for draw or win else game is continuing
            if (checkWin(board, n, sign)) {
                if (sign == 'X'){
                    printf("Player %c wins!\n\n", sign);
                    fprintf(gameState, "\nPlayer %c won!\n", sign);
                } else {
                    printf("Computer wins!\n\n");
                    fprintf(gameState, "\nComputer won!\n");
                }
                return true;
            }
            else if (checkDraw(board, n, mode)) {
                printf("It's a draw!\n\n");
                fprintf(gameState, "\nIt's a draw!\n");
                return true;
            } else {
                fprintf(gameState, "\nGame Ongoing...");
                fprintf(gameState, "\n--------------------------\n");
            }
            break;

        case 3:
            // looping 3 times to access players array
            for (int i = 0; i < 3; i++) {
                if (players[i].playerSign == sign) {
                    currentPlayerIndex = i;
                    break;
                }
            }

            // determine if current player is human or computer using currentPlayerIndex value
            if (players[currentPlayerIndex].isHuman) {
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                fprintf(gameState, "Player %c's turn.\n", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);

                printf("\n");
                // input validation
                validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);
            } else {
                printf("\nComputer (Player %c)'s turn: \n", sign);
                fprintf(gameState, "Computer (Player %c)'s turn.\n", sign);
                // passing variables to computerMove function that will decide what move to do
                computerMove(gameState, board, n, sign, 3);
                printf("\n");
            }

            // checking for endgame conditions
            if (checkWin(board, n, sign) == 1){
                if (players[currentPlayerIndex].isHuman) {
                    printf("Player %c Wins!\n\n", sign);
                    fprintf(gameState, "\nPlayer %c won!\n", sign);
                } else {
                    printf("Computer (Player %c) Wins!\n\n", sign);
                    fprintf(gameState, "\nComputer (Player %c) won!\n", sign);
                }
                return true;

            } else if (checkDraw(board, n, mode) == 1){
                printf("It's a draw!\n\n");
                fprintf(gameState, "\nIt's a draw!\n");
                return true;

            } else {
                fprintf(gameState, "\nGame Ongoing...");
                fprintf(gameState, "\n--------------------------\n");
            }

            break;
    }
    return false; // if true is not returned in any of the cases, this is returned so that the game continues
}

void validMove(FILE *gameState, char **board, int n, char sign, int *mRow, int *mColumn, int *inputResult){
    while (true){ // keeping while loop true until all validation checks have been passed
        
        // checking for non-numeric input in the coordinates user enters
        while (*inputResult != 2){
            printf("Enter numbers only. Please try again\n");
            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
        }

        // checking if move is within the range of NxN board size selected by the user
        if ((*mRow > n || *mRow <= 0) || (*mColumn > n || *mColumn <= 0)){
            printf("Invalid move. Please try again\n");

            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
            continue;
        }

        // checking if a move is already taken by user or opponent
        if (*(*(board + (*mRow-1)) + (*mColumn-1)) != '-'){
            printf("Move already taken. Please try again\n");

            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
            continue;
        }

        break; // exits while loop if all conditions are satisfied
    }

    // using pointer arithmetic to access specific element in array and change it to the current user sign
    *(*(board+(*mRow-1)) + (*mColumn-1)) = sign;
    // printing the move chosen into GameState.txt
    fprintf(gameState, "Player %c made a move at row - %d, column - %d.", sign, *mRow, *mColumn);

    char buffCorr;
    while ((buffCorr = getchar()) != '\n');
}

char switchSign(char sign, int mode){
    
    // switch-case statement to change the sign after each player makes a move
    switch (mode){
        case 1:
            // switching sign for user vs user game mode
            if (sign == 'X'){
                return 'O';
            } else {
                return 'X';
            }
            break;

        case 2:
            // switching for user vs computer game mode
            if (sign == 'X'){
                return 'O';
            } else {
                return 'X';
            }
            break;

        case 3:
            // switching for 3 player game mode
            if (sign == 'X'){
                return 'O';
            } else if (sign == 'O') {
                return 'Z';
            } else {
                return 'X';
            }
            break;
    }
}

