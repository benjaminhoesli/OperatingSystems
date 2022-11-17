#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {

  for(int row=0;row<9;row++){
    int count=0;
    bool unique=true;
    for (int i=0; i<9; i++){
      for (int x=0; x<9; x++){
        if (board[row][x]==board[row][i] && board[row][x]!=0){
          count+=1;
        }
      }
      if (count>1){
        unique=false;
      }
      count=0;
    }
    row_check[row]=unique;
  }
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
  for(int col=0;col<9;col++){
    int count=0;
    bool unique=true;
    for (int i=0; i<9; i++){
      for (int x=0; x<9; x++){
        if (board[x][col]==board[i][col] && board[x][col]!=0){
          count+=1;
        }
      }
      if (count>1){
        unique=false;
      }
      count=0;
    }
    col_check[col]=unique;
  }
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    for (int box=0; box<9; box++){
      int count=0;
      bool unique=true;
      if (box<3){
          int curr[9][2]={{0,box*3},{1,box*3},{2,box*3},{0,(box*3)+1},{1,(box*3)+1},{2,(box*3)+1},{0,(box*3)+2},{1,(box*3)+2},{2,(box*3)+2}};
          for (int x=0;x<9;x++){
            for (int i=0; i<9;i++){
              if (board[curr[i][0]][curr[i][1]]==board[curr[x][0]][curr[x][1]]&&board[curr[x][0]][curr[x][1]]!=0){
                count+=1;
              }
            }
        if (count>1){
        unique=false;
        }
        count=0;
        }
      box_check[box]=unique;
    }
      else if (box<6){
          int curr[9][2]={{3,(box-3)*3},{4,(box-3)*3},{5,(box-3)*3},{3,((box-3)*3)+1},{4,((box-3)*3)+1},{5,((box-3)*3)+1},{3,((box-3)*3)+2},{4,((box-3)*3)+2},{5,((box-3)*3)+2}};
          for (int x=0;x<9;x++){
            for (int i=0; i<9;i++){
              if (board[curr[i][0]][curr[i][1]]==board[curr[x][0]][curr[x][1]]&&board[curr[x][0]][curr[x][1]]!=0){
                count+=1;
              }
            }
        if (count>1){
        unique=false;
        }
        count=0;
        }
      box_check[box]=unique;
    }
      else {
          int curr[9][2]={{6,(box-6)*3},{7,(box-6)*3},{8,(box-6)*3},{6,((box-6)*3)+1},{7,((box-6)*3)+1},{8,((box-6)*3)+1},{6,((box-6)*3)+2},{7,((box-6)*3)+2},{8,((box-6)*3)+2}};
          for (int x=0;x<9;x++){
            for (int i=0; i<9;i++){
              if (board[curr[i][0]][curr[i][1]]==board[curr[x][0]][curr[x][1]]&&board[curr[x][0]][curr[x][1]]!=0){
                count+=1;
              }
            }
        if (count>1){
        unique=false;
        }
        count=0;
        }
      box_check[box]=unique;
    }
}
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t thread_row;
    pthread_t thread_col;
    pthread_t thread_box;
    // 3. Create a thread for each cell of each matrix operation.
    pthread_create(&thread_row, NULL, checkRow, &board);
    pthread_create(&thread_col, NULL, checkCol, &board);
    pthread_create(&thread_box, NULL, checkBox, &board);

    // 4. Wait for all threads to finish.
    pthread_join(thread_row, NULL);
    pthread_join(thread_col, NULL);
    pthread_join(thread_box, NULL);
    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}
