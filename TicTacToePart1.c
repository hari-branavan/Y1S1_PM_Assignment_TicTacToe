#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Function prototypes

char** createBoard(int n);
void displayBoard(char **board, int n);
void freeBoard(char **board, int n);
char** validMove(FILE *gameState, char **board, int n, char sign, int mRow, int mColumn, int inputResult);
char switchSign(char sign);
bool checkWin(char **board, int n);
bool checkDraw(char **board, int n);

//Main function

int main(){	
	// Declaring required variables    
	int n, mRow, mColumn, inputResult;
	char sign = 'X';
	bool gameOver = false;

	//Opening file for storing game state
	FILE *gameState = fopen("GameState.txt", "w");

    	//Error handling for opening file to write
	if (gameState == NULL){
		printf("Unable to open file for writing\n");
		return 1;
	}
    	
	//Prompting user to enter grid size
    	printf("Enter the grid size (3 <= N <= 10): ");
	int sizeResult = scanf("%d", &n);
    	
	//Validating grid size and re-prompting if necessary
   	while (n < 3 || n > 10 || sizeResult == 0){
		printf("\nInvalid size input\n\n");
	    	
		char buffch = getchar();
		while (buffch != '\n'){
			buffch = getchar();
		}

		printf("Enter the grid size (3 <= N <= 10): ");
        	sizeResult = scanf("%d", &n);
    	}

	//Initializing board
    	char **board = createBoard(n);
	
	//Displaying empty board before game
    	printf("\n");
	displayBoard(board, n);

    	while (gameOver == false){
		//Displaying whose turn it is based on current sign and logging it to text file
		if (sign == 'X'){
		    	printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
		    	fprintf(gameState, "Player %c's turn.\n", sign);
		    	
			inputResult = scanf("%d %d", &mRow, &mColumn);
        	} else {
			printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
			fprintf(gameState, "Player %c's turn.\n", sign);
		
			inputResult = scanf("%d %d", &mRow, &mColumn);
        	}

        	printf("\n");
		
		//Validating move to see if its a number and within range of board size
        	board = validMove(gameState, board, n, sign, mRow, mColumn, inputResult);
        	
		//Checking for draw or win else game is continuing
        	if (checkDraw(board, n) == 1){
			printf("It's a draw!\n\n");
			fprintf(gameState, "\nIt's a draw!\n");

			return(0);
        	
		} else if (checkWin(board,n) == 1){
			displayBoard(board, n);
			printf("\nPlayer %c Wins!\n\n", sign);
			fprintf(gameState, "\nPlayer %c won!\n", sign);

                        return(0);

		} else {
			fprintf(gameState, "\nGame Ongoing...");
                	fprintf(gameState, "\n--------------------------\n");
		}

		//Changing player sign and displaying board after the move was made
        	sign = switchSign(sign);
        	displayBoard(board, n);        
    	}
	
	//Freeing allocated memory and closing text file after use
    	freeBoard(board, n);
    	fclose(gameState);
}


//Functions

//This function allocates memory for the rows and columns and then initializes the board with '-'
char** createBoard(int n){
	char **board = malloc(n * sizeof(char*));
	
	if (!board){
		printf("Memory allocation failed!\n");
		exit(1);
	}

    	for (int i = 0; i < n; i++){
        	*(board + i) = malloc(n * sizeof(char));    
		
		if ( !(*(board+i)) ){
			printf("Memory allocation failed!\n");
			exit(1);
		}

        	for (int j = 0; j < n; j++){
            		*(*(board + i)+j) = '-';   
        	}
    	}

    	return board;
}

//This function displays the board. It has been custom-made to look user-friendly.
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

//This function is to free the allocated memory from rows and columns
void freeBoard(char **board, int n){
	for (int i = 0; i < n; i++){
        	free(board[i]);
    	}

    	free(board);
}

//This function checks for integers, whether move is already taken and whether the row and column entered are each less than 'n'
char** validMove(FILE *gameState, char **board, int n, char sign, int mRow, int mColumn, int inputResult){
    	//Checking for integer input
	while (inputResult == 0 || inputResult == 1){
		printf("Enter numbers only. Please try again\n");
	    	
		//To clear input buffer as characters are still remaining 
	    	int ch = getchar();
	    	while (ch != '\n'){
			ch = getchar();
	    	}
	    	
	    	printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
	    	inputResult = scanf("%d %d", &mRow, &mColumn);
	    	printf("\n");
    	}
	
	//Checking for valid range of row and column entered
    	while ((mRow > n || mRow <= 0) || (mColumn > n || mColumn <= 0)){
	    	printf("Invalid move. Please try again\n");
	    	printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
	    	inputResult = scanf("%d %d", &mRow, &mColumn);
	    	printf("\n");
    	}
	
	//Checking if move is already taken
    	while (*(*(board + (mRow-1)) + (mColumn-1)) != '-'){
	    	printf("Move already taken. Please try again\n");
	    	printf("\nPlayer %c's turn. Please enter coordinate wise - [row][space][column]: ", sign);
	    	inputResult = scanf("%d %d", &mRow, &mColumn);
	    	printf("\n");
    	}
    	
	//Assigning X or O to respective row:column if all checks have been passed and logging move in text file
    	*(*(board+(mRow-1)) + (mColumn-1)) = sign;
    	fprintf(gameState, "Player %c made a move at row - %d, column - %d.", sign, mRow, mColumn);
    	return board;
}

//This function switches the player sign so that the next player can make the move
char switchSign(char sign){
	
	if (sign == 'X'){
		return 'O';
	} else {
		return 'X';
    	}
}

//This function checks for win conditions
bool checkWin(char **board, int n){

// Checking for Horizontal Wins
	for (int row = 0; row < n; row++){
	    	for (int col = 0; col < n-2; col++){
		    	if (board[row][col] != '-' && 
					(board[row][col] == board[row][col+1] && 
					 board[row][col+1] == board[row][col+2])
					){
				return true;
		    	}
	    	}
    	}

// Checking for Vertical Wins

   	for (int row = 0; row < n-2; row++){
	   	for (int col = 0; col < n; col++){
		   	if (board[row][col] != '-' &&
					(board[row][col] == board[row+1][col] &&
					 board[row+1][col] == board[row+2][col])
					){
				return true;
		   	}
	   	}
   	}

// Checking for Diagonal Wins

    	for (int row = 0; row < n-2; row++){
	    	for (int col = 0; col < n-2; col++){
		    	if ((board[row][col] != '-' && 
						(board[row][col] == board[row+1][col+1] && 
						 board[row+1][col+1] == board[row+2][col+2])) ||
					(board[row][col+2] != '-' &&
					 (board[row][col+2] == board[row+1][col+1] &&
					  board[row+1][col+1] == board[row+2][col])
					 )
					){
				return true;
		    	}
	    	}
    	}
	
	return false;
}

//This function checks for draw conditions
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
