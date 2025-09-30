#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <stdio.h>
#include <stdbool.h>

// Structures

// stores the row and column that computer must move
typedef struct {
    int row;
    int col;
} Move;

typedef struct {
    char playerSign; // stores player sign X, O or Z
    bool isHuman; // true for human, false for computer
} Player;

//Function prototypes

char** createBoard(int n);
void displayBoard(char **board, int n);
void freeBoard(char **board, int n);
bool playTurn(FILE *gameState, int n, int mode, int mRow, int mColumn, char sign, int inputResult, char **board, Player players[]);
void validMove(FILE *gameState, char **board, int n, char sign, int *mRow, int *mColumn, int *inputResult);
char switchSign(char sign, int mode);
bool checkWin(char **board, int n, char sign);
bool checkDraw(char **board, int n, int mode);
void computerMove(FILE *gameState, char **board, int n, char sign);
Move tryComputerWin(char **board, char sign, int n);
Move tryComputerBlock(char **board, char sign, int n);
void ThreePlayerConfig(Player players[]);

#endif
