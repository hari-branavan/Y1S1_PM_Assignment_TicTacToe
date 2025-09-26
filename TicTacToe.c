#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Structures

typedef struct {
    int row;
    int col;
} Move;

typedef struct {
    char playerSign;
    bool isHuman; // true for human, false for computer
} Player;

//Function prototypes

char** createBoard(int n);
void displayBoard(char **board, int n);
void freeBoard(char **board, int n);
bool playTurn(FILE *gameState, int n, int mode, int mRow, int mColumn, char sign, int inputResult, char **board, Player players[]);
void validMove(FILE *gameState, char **board, int n, char sign, int *mRow, int *mColumn, int *inputResult);
char switchSign(char sign, int mode);
bool checkWin(char **board, int n);
bool checkDraw(char **board, int n);
void computerMove(FILE *gameState, char **board, int n, char sign);
Move tryComputerWin(char **board, char sign, int n);
Move tryComputerBlock(char **board, char sign, int n);
void ThreePlayerConfig(Player players[]);

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
    
    displayBoard(board, n);
    
    while (!gameOver){
        gameOver = playTurn(gameState, n, mode, mRow, mColumn, sign, inputResult, board, players);
        
        sign = switchSign(sign, mode);
        displayBoard(board, n);
        printf("\n");
    }

    freeBoard(board, n);
    fclose(gameState);
    return 0;
}

//Functions

void ThreePlayerConfig(Player players[]){
    printf("\nAt least one player must be human as required.\n\n");
    printf("\nConfiguring 3-player mode:\n");
    printf("Player X goes first\n");
    printf("Player O goes second\n");
    printf("Player Z goes third\n\n");

    players[0].playerSign = 'X';
    players[1].playerSign = 'O';
    players[2].playerSign = 'Z';

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
    }

    printf("3 Player game configuration complete!\n\n");
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

bool playTurn(FILE *gameState, int n, int mode, int mRow, int mColumn, char sign, int inputResult, char **board, Player players[]){
    int currentPlayerIndex = -1;

    switch (mode){
        case 1:
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

            if (checkWin(board,n) == 1){
                printf("Player %c Wins!\n\n", sign);
                fprintf(gameState, "\nPlayer %c won!\n", sign);
                return true;
            } else if (checkDraw(board, n) == 1){
                printf("It's a draw!\n");
                fprintf(gameState, "\nIt's a draw!\n");
                return true;
            } else {
                fprintf(gameState, "\nGame Ongoing...");
                fprintf(gameState, "\n--------------------------\n");
            }

            break;
        
        case 2:
            if (sign == 'X'){
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
            if (checkWin(board, n)) {
                if (sign == 'X'){
                    printf("\nPlayer %c wins!\n\n", sign);
                    fprintf(gameState, "\nPlayer %c won!\n", sign);
                } else {
                    printf("\nComputer wins!\n\n");
                    fprintf(gameState, "\nComputer won!\n");
                }
                return true;
            }
            else if (checkDraw(board, n)) {
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

            if (checkWin(board,n) == 1){
                if (players[currentPlayerIndex].isHuman) {
                    printf("Player %c Wins!\n\n", sign);
                    fprintf(gameState, "\nPlayer %c won!\n", sign);
                } else {
                    printf("Computer (Player %c) Wins!\n\n", sign);
                    fprintf(gameState, "\nComputer (Player %c) won!\n", sign);
                }
                return true;
            } else if (checkDraw(board, n) == 1){
                printf("It's a draw!\n");
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
    while (*inputResult != 2){
        printf("Enter numbers only. Please try again\n");
        
        int c = getchar();
        while (c != '\n'){
            c = getchar();
        }
        
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        *inputResult = scanf("%d %d", mRow, mColumn);
        printf("\n");
    }

    while ((*mRow > n || *mRow <= 0) || (*mColumn > n || *mColumn <= 0)){
        printf("Invalid move. Please try again\n");
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        *inputResult = scanf("%d %d", mRow, mColumn);
        printf("\n");
        
        // Check if input was successful
        while (*inputResult != 2){
            printf("Enter numbers only. Please try again\n");
            
            int c = getchar();
            while (c != '\n'){
                c = getchar();
            }
            
            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
        }
    }

    while (*(*(board + (*mRow-1)) + (*mColumn-1)) != '-'){
        printf("Move already taken. Please try again\n");
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        *inputResult = scanf("%d %d", mRow, mColumn);
        printf("\n");
        
        // Check if input was successful and valid coordinates
        while (*inputResult != 2){
            printf("Enter numbers only. Please try again\n");
            
            int c = getchar();
            while (c != '\n'){
                c = getchar();
            }
            
            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
        }
        
        while ((*mRow > n || *mRow <= 0) || (*mColumn > n || *mColumn <= 0)){
            printf("Invalid move. Please try again\n");
            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            *inputResult = scanf("%d %d", mRow, mColumn);
            printf("\n");
            
            while (*inputResult != 2){
                printf("Enter numbers only. Please try again\n");
                
                int c = getchar();
                while (c != '\n'){
                    c = getchar();
                }
                
                printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
                *inputResult = scanf("%d %d", mRow, mColumn);
                printf("\n");
            }
        }
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

bool checkWin(char **board, int n){
    // Diagonal check
    for (int row = 0; row < n-2; row++){
        for (int col = 0; col < n-2; col++){
            if ((board[row][col] != '-' && (board[row][col] == board[row+1][col+1] && board[row+1][col+1] == board[row+2][col+2])) ||
                (board[row][col+2] != '-' && (board[row][col+2] == board[row+1][col+1] && board[row+1][col+1] == board[row+2][col])))
                {
                    return true;
            }
        }
    }
    // Horizontal check
    for (int row = 0; row < n; row++){
        for (int col = 0; col < n-2; col++){
            if (board[row][col] != '-' && 
                board[row][col] == board[row][col+1] && 
                board[row][col+1] == board[row][col+2])
                {
                    return true;
            }
        }
    }
    // Vertical check
    for (int row = 0; row < n-2; row++){
        for (int col = 0; col < n; col++){
            
            if (board[row][col] != '-' && 
                board[row][col] == board[row+1][col] && 
                board[row+1][col] == board[row+2][col])
                {
                    return true;
            }
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

Move tryComputerWin(char **board, char sign, int n){
    Move coords = {-1, -1};

    // Horizontal Check
    for (int row = 0; row < n; row++){
        for (int col = 0; col < n-2; col++){
            // OO-
            if (board[row][col] == sign && board[row][col+1] == sign && board[row][col+2] == '-') {
                coords.row = row;
                coords.col = col+2;
                return coords;
            }
            // O-O
            if (board[row][col] == sign && board[row][col+1] == '-' && board[row][col+2] == sign) {
                coords.row = row;
                coords.col = col+1;
                return coords;
            }
            // -OO
            if (board[row][col] == '-' && board[row][col+1] == sign && board[row][col+2] == sign) {
                coords.row = row;
                coords.col = col;
                return coords;
            }
        }
    }

    // Vertical Check
    for (int row = 0; row < n-2; row++){
        for (int col = 0; col < n; col++){
            // OO-
            if (board[row][col] == sign && board[row+1][col] == sign && board[row+2][col] == '-') {
                coords.row = row+2;
                coords.col = col;
                return coords;
            }
            // O-O
            if (board[row][col] == sign && board[row+1][col] == '-' && board[row+2][col] == sign) {
                coords.row = row+1;
                coords.col = col;
                return coords;
            }
            // -OO
            if (board[row][col] == '-' && board[row+1][col] == sign && board[row+2][col] == sign) {
                coords.row = row;
                coords.col = col;
                return coords;
            }
        }
    }

    // Diagonal Check (top-left to bottom-right)
    for (int row = 0; row < n-2; row++){
        for (int col = 0; col < n-2; col++){
            // OO-
            if (board[row][col] == sign && board[row+1][col+1] == sign && board[row+2][col+2] == '-') {
                coords.row = row+2;
                coords.col = col+2;
                return coords;
            }
            // O-O
            if (board[row][col] == sign && board[row+1][col+1] == '-' && board[row+2][col+2] == sign) {
                coords.row = row+1;
                coords.col = col+1;
                return coords;
            }
            // -OO
            if (board[row][col] == '-' && board[row+1][col+1] == sign && board[row+2][col+2] == sign) {
                coords.row = row;
                coords.col = col;
                return coords;
            }
        }
    }

    // Diagonal Check (top-right to bottom-left)
    for (int row = 0; row < n-2; row++){
        for (int col = 2; col < n; col++){
            // OO-
            if (board[row][col] == sign && board[row+1][col-1] == sign && board[row+2][col-2] == '-') {
                coords.row = row+2;
                coords.col = col-2;
                return coords;
            }
            // O-O
            if (board[row][col] == sign && board[row+1][col-1] == '-' && board[row+2][col-2] == sign) {
                coords.row = row+1;
                coords.col = col-1;
                return coords;
            }
            // -OO
            if (board[row][col] == '-' && board[row+1][col-1] == sign && board[row+2][col-2] == sign) {
                coords.row = row;
                coords.col = col;
                return coords;
            }
        }
    }

    return coords; // no winning move found
}

Move tryComputerBlock(char **board, char sign, int n){ 
    Move coords = {-1, -1};
    char opponent;
    char possibleOpponents[] = {'X','O','Z'};

    for (int opIndex = 0; opIndex < 3; opIndex++){
        opponent = possibleOpponents[opIndex];

        if (opponent == sign){
            continue;
        }

        bool opponentExists = false;
        for (int i = 0; i < n && !opponentExists; i++){
            for (int j = 0; j < n && !opponentExists; j++){
                if (board[i][j] == opponent){
                    opponentExists = true;
                }
            }
        }

        if (!opponentExists){
            continue;
        }

        // Horizontal Check
        for (int row = 0; row < n; row++){
            for (int col = 0; col < n-2; col++){
                // XX-
                if (board[row][col] == opponent && board[row][col+1] == opponent && board[row][col+2] == '-') {
                    coords.row = row;
                    coords.col = col+2;
                    return coords;
                }
                // X-X
                if (board[row][col] == opponent && board[row][col+1] == '-' && board[row][col+2] == opponent) {
                    coords.row = row;
                    coords.col = col+1;
                    return coords;
                }
                // -XX
                if (board[row][col] == '-' && board[row][col+1] == opponent && board[row][col+2] == opponent) {
                    coords.row = row;
                    coords.col = col;
                    return coords;
                }
            }
        }

        // Vertical Check
        for (int row = 0; row < n-2; row++){
            for (int col = 0; col < n; col++){
                // XX-
                if (board[row][col] == opponent && board[row+1][col] == opponent && board[row+2][col] == '-') {
                    coords.row = row+2;
                    coords.col = col;
                    return coords;
                }
                // X-X
                if (board[row][col] == opponent && board[row+1][col] == '-' && board[row+2][col] == opponent) {
                    coords.row = row+1;
                    coords.col = col;
                    return coords;
                }
                // -XX
                if (board[row][col] == '-' && board[row+1][col] == opponent && board[row+2][col] == opponent) {
                    coords.row = row;
                    coords.col = col;
                    return coords;
                }
            }
        }

        // Diagonal Check (top-left to bottom-right)
        for (int row = 0; row < n-2; row++){
            for (int col = 0; col < n-2; col++){
                // XX-
                if (board[row][col] == opponent && board[row+1][col+1] == opponent && board[row+2][col+2] == '-') {
                    coords.row = row+2;
                    coords.col = col+2;
                    return coords;
                }
                // X-X
                if (board[row][col] == opponent && board[row+1][col+1] == '-' && board[row+2][col+2] == opponent) {
                    coords.row = row+1;
                    coords.col = col+1;
                    return coords;
                }
                // -XX
                if (board[row][col] == '-' && board[row+1][col+1] == opponent && board[row+2][col+2] == opponent) {
                    coords.row = row;
                    coords.col = col;
                    return coords;
                }
            }
        }

        // Diagonal Check (top-right to bottom-left)
        for (int row = 0; row < n-2; row++){
            for (int col = 2; col < n; col++){
                // XX-
                if (board[row][col] == opponent && board[row+1][col-1] == opponent && board[row+2][col-2] == '-') {
                    coords.row = row+2;
                    coords.col = col-2;
                    return coords;
                }
                // X-X
                if (board[row][col] == opponent && board[row+1][col-1] == '-' && board[row+2][col-2] == opponent) {
                    coords.row = row+1;
                    coords.col = col-1;
                    return coords;
                }
                // -XX
                if (board[row][col] == '-' && board[row+1][col-1] == opponent && board[row+2][col-2] == opponent) {
                    coords.row = row;
                    coords.col = col;
                    return coords;
                }
            }
        }
    }

    return coords; // no block found
}

void computerMove(FILE *gameState, char **board, int n, char sign){
    int row, col;

    Move co_ords = tryComputerWin(board, sign, n);
    
    if (co_ords.row == -1 && co_ords.col == -1){
        co_ords = tryComputerBlock(board, sign, n);
    }

    if (co_ords.row == -1 && co_ords.col == -1){
        do{
            row = rand() % n;
            col = rand() % n;
        } while (*(*(board + row) + col) != '-');

    } else {
        row = co_ords.row;
        col = co_ords.col;
    }

    *(*(board + row) + col) = sign;
    fprintf(gameState, "Computer made a move at row - %d, column - %d.", row+1, col+1);   
}
