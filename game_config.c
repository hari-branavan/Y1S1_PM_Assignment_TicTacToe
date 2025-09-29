#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "game_init.h"

void ThreePlayerConfig(Player players[]){
    printf("\nAt least one player must be human as required.\n");
    printf("\nConfiguring 3-player mode:\n");
    printf("Player X goes first\n");
    printf("Player O goes second\n");
    printf("Player Z goes third\n\n");

    players[0].playerSign = 'X';
    players[1].playerSign = 'O';
    players[2].playerSign = 'Z';

    int humanCount = 0;

    for (int i = 0; i < 3; i++){
        int choice;
        printf("Configure Player %c:\n", players[i].playerSign);
        printf("1. Human Player\n");
        printf("2. Computer Player\n");
        printf("Enter choice (1-2): ");

        int choiceResult = scanf("%d", &choice);

        while (choice < 1 || choice > 2 || choiceResult != 1) {
            printf("\nInvalid choice. Please enter 1 for Human or 2 for Computer: ");

            // Clear input buffer
            char buff = getchar();
            while (buff != '\n') {
                buff = getchar();
            }

            choiceResult = scanf("%d", &choice);
        }

        players[i].isHuman = (choice == 1);

        if (players[i].isHuman) {
            printf("\nPlayer %c configured as Human Player\n\n", players[i].playerSign);
        } else {
            printf("\nPlayer %c configured as Computer Player\n\n", players[i].playerSign);
        }

        if (choice == 1){
            humanCount++;
        }
    }

    if (humanCount == 0){
        char secChoice;
        printf("Atleast one player must be human! Please enter which player (X, O, Z) you want to be human: ");
        scanf(" %c", &secChoice);

        if (tolower(secChoice) == 'x'){
            players[0].isHuman = true;
        } else if (tolower(secChoice)  == 'o') {
            players[1].isHuman = true;
        } else if (tolower(secChoice) == 'z'){
            players[2].isHuman = true;
        }
        printf("\n");
    }

    printf("3 Player game configuration complete!\n");
}


char** createBoard(int n){
    char **board = malloc(n * sizeof(char*));

    if (!board) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++){
        *(board + i) = malloc(n * sizeof(char));

        if (!(*(board + i))) {
            printf("Memory allocation failed!\n");
            exit(1);
        }

        for (int j = 0; j < n; j++){
            *(*(board + i)+j) = '-';
        }
    }

    return board;
}

void displayBoard(char **board, int n){
    for (int i = 0; i < n; i++){
        if (i == 0){
            printf("    %d", i+1);
        } else {
            printf("   %d", i+1);
        }
    }

    printf("\n");

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (j == 0){
                printf("  +---");
            } else {
                printf("+---");
            }
        }
        printf("+\n%d ", i+1);

        for (int j = 0; j < n; j++){
            printf("| %c ", *(*(board + i)+j));
        }
        printf("|\n");
    }

    for (int i = 0; i < n; i++){
        if (i == 0){
            printf("  +---");
        } else {
            printf("+---");
        }
    }

    printf("+\n");
}

void freeBoard(char **board, int n){
    for (int i = 0; i < n; i++){
        free(board[i]);
    }

    free(board);
}

