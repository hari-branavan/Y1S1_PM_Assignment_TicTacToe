#include <stdio.h>

void boardPrint(int n, char board[n][n]);

int main(){
    int n;
    
    printf("Enter the grid size (NxN): ");
    scanf("%d", &n);

    char board[n][n];
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            board[i][j] = '-';
        }
    }
    
    boardPrint(n, board);

}

void boardPrint(int n, char board[n][n]){
    for (int i = 0; i < n; i++){
        if (i == 0){
            printf("    %d", i);
        } else {
            printf("   %d", i);
        }
    }
    printf("\n  ");
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < n; j++){
            if (j == 0){
                printf("%d | %c ", i, board[i][j]);
            } else {
                printf("| %c ", board[i][j]);
            }
        }
        printf("|\n  ");
    }

    for (int i = 0; i < n; i++){
        printf("+---");
    }
    printf("+\n");
}
