#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "game_init.h"

// this function gives instructions on how to play the game.
void helper(){
    printf("Welcome to TIC-TAC-TOE!!!\n");
    printf("You will first be prompted to enter the grid size. Please choose between integers 3 and 10.\n");
    printf("Then you will choose a game mode. Pick 1, 2, or 3.\n");
    printf("In 3-player mode, you will have to define the roles of each player X, O, Z as human or computer.\n");
    printf("The empty board will then be shown to you.\n");
    printf("When entering moves, enter the row number first, add a space and then the column number.\n");
    printf("Invalid moves include non-numeric characters and out of range numbers.\n");
    printf("You will be prompted again if you enter these.\n");
    printf("May The Best Player Win!!!\n\n");
}

// this function is to configure players as human or computer in 3 player mode
void ThreePlayerConfig(Player players[]){
    // printing requirements on console
    printf("\nAt least one player must be human as required.\n");
    printf("\nConfiguring 3-player mode:\n");
    printf("Player X goes first\n");
    printf("Player O goes second\n");
    printf("Player Z goes third\n\n");
    
    // harcoding the signs into player array of Player structure
    players[0].playerSign = 'X';
    players[1].playerSign = 'O';
    players[2].playerSign = 'Z';

    // variable to count how many users are human(used to force user to enter 1 human player)
    int humanCount = 0;
    
    // looping 3 times to configure all 3 players
    for (int i = 0; i < 3; i++){
        int choice; // used to stored 1 or 2 for human or computer respectively
        
        // user prompt to enter choice
        printf("Configure Player %c:\n", players[i].playerSign);
        printf("1. Human Player\n");
        printf("2. Computer Player\n");
        printf("Enter choice (1-2): ");
        
        // variable used for input validation
        int choiceResult = scanf("%d", &choice);

        // validating choice input
        while (choice < 1 || choice > 2 || choiceResult != 1) {
            printf("\nInvalid choice. Please enter 1 for Human or 2 for Computer: ");

            // clear input buffer so while loop doesnt forever take in those same values
            char buff = getchar();
            while (buff != '\n') {
                buff = getchar();
            }

            // storing scanf result again for validation
            choiceResult = scanf("%d", &choice);
        }

        // using ternary operator to store boolean value to isHuman variable in Player structure
        players[i].isHuman = (choice == 1);

        // clear input buffer so user cant enter more than 2 correct digits
        char buff;
        while ((buff = getchar()) != '\n');

        // showing which players have been configured as human or computer
        if (players[i].isHuman) {
            printf("\nPlayer %c configured as Human Player\n\n", players[i].playerSign);
        } else {
            printf("\nPlayer %c configured as Computer Player\n\n", players[i].playerSign);
        }

        // incrementing humanCount variable
        if (choice == 1){
            humanCount++;
        }
    }

    // checking humanCount to see if it is still zero after user has chosen 3 times
    if (humanCount == 0){
        char secChoice; // declaring second choice variable
        printf("Atleast one player must be human! Please enter which player (X, O, Z) you want to be human: ");
        scanf(" %c", &secChoice);

        // user enters character x, o or z so boolean value is assigned to isHuman in Player structure
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

// function to allocate memory for each element in the board and return the board
char** makeBoard(int n){
    // allocating for each row
    char **board = malloc(n * sizeof(char*));

    if (!board) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // allocating for each column
    for (int i = 0; i < n; i++){
        *(board + i) = malloc(n * sizeof(char));

        if (!(*(board + i))) {
            printf("Memory allocation failed!\n");
            exit(1);
        }

        // to make it user friendly '-' was added to show which boxes arent taken
        for (int j = 0; j < n; j++){
            *(*(board + i)+j) = '-';
        }
    }

    return board;
}

// function to print board and make it look user friendly
void displayBoard(char **board, int n){

    // prints the column numbers first. IF conditions have been used for formatting purposes
    for (int i = 0; i < n; i++){
        if (i == 0){
            printf("     %d", i+1);
        } else {
            printf("   %d", i+1);
        }
    }

    printf("\n");

    // for loop to print the rest of the board
    for (int i = 0; i < n; i++){
        // for loop to print the borders of the board
        for (int j = 0; j < n; j++){
            if (i == 0){
                if (j == 0){
                    printf("   +---");
                } else {
                    printf("+---");
                }
            } else {
                if (j == 0){
                    printf("  +---");
                } else {
                    printf("+---");
                }
            }
        }
        
        // IF condition used for formatting purposes for 10x10 grid
        if (i == 9){
            printf("+\n%d ", i+1); // prints row number
        } else {
            printf("+\n %d ", i+1); // prints row number
        }

        // also for displaying the border
        for (int j = 0; j < n; j++){
            printf("| %c ", *(*(board + i)+j));
        }

        // display last | and then new line
        printf("|\n ");
    }

    // to print bottom border. IF conditions for formatting
    for (int i = 0; i < n; i++){
        if (i == 0){
            printf("  +---");
        } else {
            printf("+---");
        }
    }

    // last + and then new line
    printf("+\n");
}

// function to clear memory allocated to prevent memory leaks
void freeBoard(char **board, int n){
    for (int i = 0; i < n; i++){
        free(board[i]);
    }

    free(board);
}
