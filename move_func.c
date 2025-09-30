#include <stdio.h>
#include <stdbool.h>

#include "game_init.h"

bool playTurn(FILE *gameState, int n, int mode, int mRow, int mColumn, char sign, int inputResult, char **board, Player players[]){
    int currentPlayerIndex = -1;
    
    switch (mode){
        case 1:
            char buffer;
            while ((buffer = getchar()) != '\n');

            if (sign == 'X'){
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);
            } else {
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);
            }

            fprintf(gameState, "Player %c's turn.\n", sign);

            printf("\n");
            validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);

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
            if (sign == 'X'){
                char buffer;
                while ((buffer = getchar()) != '\n');

                printf("\nPlayer %c to move. Please enter coordinate wise - [row][space][column]: ", sign);
                fprintf(gameState, "Player %c's turn.\n", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);

                validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);
            } else {
                printf("\nComputer's turn: \n");
                fprintf(gameState, "Computer's turn.\n");
                computerMove(gameState, board, n, sign);
            }

            printf("\n");

            // Checking for draw or win else game is continuing
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
            for (int i = 0; i < 3; i++) {
                if (players[i].playerSign == sign) {
                    currentPlayerIndex = i;
                    break;
                }
            }

            // Determine if current player is human or computer
            if (players[currentPlayerIndex].isHuman) {
                char buffer;
                while ((buffer = getchar()) != '\n');

                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                fprintf(gameState, "Player %c's turn.\n", sign);
                inputResult = scanf("%d %d", &mRow, &mColumn);

                printf("\n");
                validMove(gameState, board, n, sign, &mRow, &mColumn, &inputResult);
            } else {
                printf("\nComputer (Player %c)'s turn: \n", sign);
                fprintf(gameState, "Computer (Player %c)'s turn.\n", sign);
                computerMove(gameState, board, n, sign);
                printf("\n");
            }

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
    return false; // Game continues
}

void validMove(FILE *gameState, char **board, int n, char sign, int *mRow, int *mColumn, int *inputResult){
    while (true){
        while (*inputResult != 2){
            printf("Enter numbers only. Please try again\n");
            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
        }

        if ((*mRow > n || *mRow <= 0) || (*mColumn > n || *mColumn <= 0)){
            printf("Invalid move. Please try again\n");

            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
            continue;
        }

        if (*(*(board + (*mRow-1)) + (*mColumn-1)) != '-'){
            printf("Move already taken. Please try again\n");

            int buffer;
            while((buffer = getchar()) != '\n');

            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
            continue;
        }

        break;
    }

    *(*(board+(*mRow-1)) + (*mColumn-1)) = sign;
    fprintf(gameState, "Player %c made a move at row - %d, column - %d.", sign, *mRow, *mColumn);
}

char switchSign(char sign, int mode){
    switch (mode){
        case 1:
            if (sign == 'X'){
                return 'O';
            } else {
                return 'X';
            }
            break;

        case 2:
            if (sign == 'X'){
                return 'O';
            } else {
                return 'X';
            }
            break;

        case 3:
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

