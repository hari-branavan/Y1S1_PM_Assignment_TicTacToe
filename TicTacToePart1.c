#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//Function prototypes


char** createBoard(int n);
void displayBoard(char **board, int n);
void freeBoard(char **board, int n);
char** validMove(char **board, int n, char sign, int mRow, int mColumn, int inputResult);
char switchSign(char sign);
bool checkWin(char **board, int n);
bool checkDraw(char **board, int n);


//Main function


int main(){
    int n, mRow, mColumn, inputResult;
    char sign = 'X';
    bool gameOver = false;

    printf("Enter the grid size (3 <= N <= 10): ");
    scanf("%d", &n);
    
    printf("\n");
    
    if (n < 3 || n > 10){
        printf("Invalid size\n\n");
        return 1;
    }

    char **board = createBoard(n);

    displayBoard(board, n);

    while (gameOver == false){
        if (sign == 'X'){
            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            inputResult = scanf("%d %d", &mRow, &mColumn);
        } else {
            printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
            inputResult = scanf("%d %d", &mRow, &mColumn);
        }

        printf("\n");

        board = validMove(board, n, sign, mRow, mColumn, inputResult);
        
        if (checkDraw(board, n) == 1){
            printf("It's a draw!\n\n");
            
            exit(0);
        }
        
        if (checkWin(board,n) == 1){
            displayBoard(board, n);
            printf("\nPlayer %c Wins!\n\n", sign);
            
            exit(0);
        }

        sign = switchSign(sign);
        displayBoard(board, n);        
    }
    freeBoard(board, n);
}


//Functions


char** createBoard(int n){
    char **board = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++){
        *(board + i) = malloc(n * sizeof(char));
    

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


char** validMove(char **board, int n, char sign, int mRow, int mColumn, int inputResult){
    while (inputResult == 0 || inputResult == 1){
        printf("Enter numbers only. Please try again\n");
        
        int c = getchar();
        while (c != '\n' && c != EOF){
            c = getchar();
        };
        
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        inputResult = scanf("%d %d", &mRow, &mColumn);
        printf("\n");
    }


    while ((mRow > n || mRow <= 0) || (mColumn > n || mColumn <= 0)){
        printf("Invalid move. Please try again\n");
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        inputResult = scanf("%d %d", &mRow, &mColumn);
        printf("\n");
    }

    while (*(*(board + (mRow-1)) + (mColumn-1)) != '-'){
        printf("Move already taken. Please try again\n");
        printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
        inputResult = scanf("%d %d", &mRow, &mColumn);
        printf("\n");
    }

    *(*(board+(mRow-1)) + (mColumn-1)) = sign;

    return board;
}


char switchSign(char sign){
    if (sign == 'X'){
        return 'O';
    } else {
        return 'X';
    }
}


bool checkWin(char **board, int n){
    
    for (int row = 0; row < n-2; row++){
        for (int col = 0; col < n-2; col++){
            
            // Horizontal check
            if (board[row][col] != '-' && 
                (board[row][col] == board[row][col+1] && 
                board[row][col+1] == board[row][col+2])){
                    
                    return true;
            }
            
            // Vertical check
            if (board[row][col] != '-' && 
                board[row][col] == board[row+1][col] && 
                board[row+1][col] == board[row+2][col])
                {
                
                    return true;
            }
            
            // Diagonal check
            if ((board[row][col] != '-' &&
		(board[row][col] == board[row+1][col+1] && 
		 board[row+1][col+1] == board[row+2][col+2])
		) 
		||
		(board[row][col+2] != '-' && 
		(board[row][col+2] == board[row+1][col+1] && 
		 board[row+1][col+1] == board[row+2][col])
		))
                {
                    return true;
            }
        }
    }
    return false;
}

bool checkDraw(char **board, int n){
    
    int unusedBox = 0;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (*(*(board+i)+j) != '-'){
                unusedBox += 1;
            }
        }
    }   
    if (unusedBox == (n*n)){
        return true;
    }
    return false;
}
