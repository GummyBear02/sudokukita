/*
 * 4x4 Sudoku generator + interactive player
 * Compile: gcc -O2 sudoku4_play.c -o sudoku4_play
 * Run:     ./sudoku4_play 6   (arg = jumlah blanks, default 6)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 4
#define BLOCK 2

/* a valid solved 4x4 board (2x2 blocks) */
int base_solved[N][N] = {
  {1, 2, 3, 4},
  {3, 4, 1, 2},
  {2, 1, 4, 3},
  {4, 3, 2, 1}
};

void copy_board(int dst[N][N], int src[N][N]) {
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      dst[row][col] = src[row][col];
    }
  }
}

/**
 * Prints a 4x4 Sudoku board in a human-readable format.
 * The board is displayed with row and column numbers,
 * and the values are displayed with the given value,
 * or a '.' if the value is 0.
 * A '+' is displayed at the top and bottom of the board,
 * and a '|' is displayed at the end of each row.
 * A '+' is also displayed in the middle of the board,
 * separating the two 2x2 blocks.
 * @param a The 4x4 Sudoku board to print.
 */
void print_board(int a[N][N]) {
  /* 
   * Print the top line of the board, with column numbers.
   * This is the line that will be displayed above the board.
   */
  printf("    1  2   3  4\n");
  printf("  +------+------+\n");

  /* 
   * Print each row of the board.
   * For each row, print the row number, followed by the values in the row.
   * A '|' is displayed at the end of each row.
   */
  for (int i=0; i<N; i++) {
    /* 
     * Print the row number.
     * This is the number that will be displayed to the left of the row.
     */
    printf("%d |", i+1);

    /* 
     * Print each value in the row.
     * If the value is 0, print a '.', otherwise print the value.
     * A space is displayed after each value.
     * A '|' is displayed after the second value in the row.
     */
    for (int j=0; j<N; j++) {
      if (a[i][j] == 0) { 
        /* 
         * Print a '.' if the value is 0.
         * This indicates that the value has not been set yet.
         */
        printf(" .");
      }
      else { 
        /* 
         * Print the value if it is not 0.
         * This is the actual value that has been set in the Sudoku board.
         */
        printf(" %d", a[i][j]);
      }
      
      /* 
       * If we're not at the end of the row, print a space.
       * This is to separate the values in the row.
       */
      if (j != N-1 ) {
        printf(" ");
      }
      
      /* 
       * If we're at the middle of the row, print a '|'.
       * This is to separate the two 2x2 blocks in the board.
       */
      if ( j == 1 ) {
        printf("|");
      }
    }
    printf(" |\n");

    /* 
     * If we're at the middle of the board, print a '+'.
     * This is to separate the two 2x2 blocks in the board.
     */
    if (i==1) {
      printf("  +------+------+\n");
    }
  }
  /* 
   * Print the bottom line of the board.
   * This is the line that will be displayed below the board.
   */
  printf("  +------+------+\n");

  return;
}

/**
 * Check if it's valid to place a value v at position (r,c) on the Sudoku board a.
 * @param a The Sudoku board to check.
 * @param r The row of the position to check.
 * @param c The column of the position to check.
 * @param v The value to check.
 * @return 1 if it's valid, 0 otherwise.
 */
int is_valid(int a[N][N], int r, int c, int v) {
  // check row
  for (int i=0; i<N; i++) {
    if (a[r][i] == v) return 0;
  }

  // check column
  for (int i=0; i<N; i++) {
    if (a[i][c] == v) return 0;
  }

  // check block
  int block_row = (r / BLOCK) * BLOCK;
  int block_col = (c / BLOCK) * BLOCK;
  for (int i=0; i<BLOCK; i++) {
    for (int j=0; j<BLOCK; j++) {
      if (a[block_row+i][block_col+j] == v) return 0;
    }
  }

  return 1;
}

/**
 * A recursive solver that counts solutions up to a given limit.
 * @param board The Sudoku board to solve.
 * @param limit The maximum number of solutions to count.
 * @param count A pointer to an integer that stores the count of solutions.
 * @return The count of solutions (up to the limit).
 */
int solve_count_recursive(int board[N][N], int limit, int *count) {
  /* If the count has already reached the limit, stop counting */
  if (*count >= limit) return *count;

  /* Find an empty cell in the board */
  int row = -1, col = -1;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      if (board[i][j] == 0) { row = i; col = j; break; }
    }
    if (row != -1) break;
  }

  /* If all cells are filled, increment the count */
  if (row == -1) { 
    (*count)++; 
    return *count; 
  }

  /* Try all possible values for the empty cell */
  for (int v=1; v<=N; v++) {
    if (is_valid(board, row, col, v)) {
      board[row][col] = v;
      solve_count_recursive(board, limit, count);
      board[row][col] = 0;
      
      if (*count >= limit) {
        return *count;
      }
    }
  }
  return *count;
}

int count_solutions(int a[N][N], int limit) {
  int tmp[N][N];
  copy_board(tmp, a);

  int cnt = 0;
  solve_count_recursive(tmp, limit, &cnt);
  
  return cnt;
}

/* simple transformations that preserve solution validity */
void swapRows(int board[N][N], int row1, int row2) {
  for (int col = 0; col < N; col++) {
    int temp = board[row1][col];
    board[row1][col] = board[row2][col];
    board[row2][col] = temp;
  }
}

void swapCols(int board[N][N], int col1, int col2) {
  for (int row = 0; row < N; row++) {
    int temp = board[row][col1];
    board[row][col1] = board[row][col2];
    board[row][col2] = temp;
  }
}

void transposeBoard(int board[N][N]) {
  for (int row = 0; row < N; row++) {
    for (int col = row + 1; col < N; col++) {
      int temp = board[row][col];
      board[row][col] = board[col][row];
      board[col][row] = temp;
    }
  }
}

void swapRowBlocks(int board[N][N], int block1, int block2) {
  for (int i = 0; i < BLOCK; i++) {
    swapRows(board, block1 * BLOCK + i, block2 * BLOCK + i);
  }
}

void swapColBlocks(int board[N][N], int block1, int block2) {
  for (int i = 0; i < BLOCK; i++) {
    swapCols(board, block1 * BLOCK + i, block2 * BLOCK + i);
  }
}

/**
 * Randomly transforms a Sudoku board in order to generate a unique
 * puzzle. The transformations are chosen randomly from the following
 * set: swapping two rows in the same block, swapping two columns in
 * the same block, swapping two blocks of rows, swapping two blocks of
 * columns, transposing the board. The number of transformations is
 * fixed at 50.
 * @param board The Sudoku board to transform.
 */
void randomizeSolution(int board[N][N]) {
  const int iterations = 50;
  const int transformations = 5;

  for (int iteration = 0; iteration < iterations; iteration++) {
    int transformation = rand() % transformations;

    switch (transformation) {
      /* 
       * Swap two rows in the same block.
       * The block is chosen randomly, and the two rows are chosen
       * randomly from within that block.
       */
      case 0: {
        int block = rand() % BLOCK;
        int row1 = block * BLOCK + rand() % BLOCK;
        int row2 = block * BLOCK + rand() % BLOCK;

        if (row1 != row2) {
          swapRows(board, row1, row2);
        }
        break;
      }

      /* 
       * Swap two columns in the same block.
       * The block is chosen randomly, and the two columns are chosen
       * randomly from within that block.
       */
      case 1: {
        int block = rand() % BLOCK;
        int col1 = block * BLOCK + rand() % BLOCK;
        int col2 = block * BLOCK + rand() % BLOCK;

        if (col1 != col2) {
          swapCols(board, col1, col2);
        }
        break;
      }

      /* 
       * Swap two blocks of rows.
       * The two blocks are chosen randomly, and all rows in the first
       * block are swapped with all rows in the second block.
       */
      case 2: {
        int block1 = rand() % BLOCK;
        int block2 = rand() % BLOCK;

        if (block1 != block2) {
          swapRowBlocks(board, block1, block2);
        }
        break;
      }

      /* 
       * Swap two blocks of columns.
       * The two blocks are chosen randomly, and all columns in the first
       * block are swapped with all columns in the second block.
       */
      case 3: {
        int block1 = rand() % BLOCK;
        int block2 = rand() % BLOCK;

        if (block1 != block2) {
          swapColBlocks(board, block1, block2);
        }
        break;
      }

      /* 
       * Transpose the board.
       * This transformation swaps the rows and columns of the board.
       */
      case 4: {
        if (rand() % 3 == 0) {
          transposeBoard(board);
        }
        break;
      }

      /* 
       * No transformation is done for this iteration.
       */
      default:
        break;
    }
  }

/* generator: remove cells while keeping unique solution */
void generate_puzzle(int a[N][N], int blanks) {
  int pos[16];
  for (int i=0;i<16;i++) pos[i]=i;
  for (int i=15;i>0;i--) {
    int j = rand()%(i+1);
    int t = pos[i]; pos[i]=pos[j]; pos[j]=t;
  }

  int removed = 0;
  for (int k=0; k<16 && removed < blanks; k++) {
    int p = pos[k];
    int r = p / N, c = p % N;
    if (a[r][c]==0) continue;
    int saved = a[r][c];
    a[r][c]=0;
    int sols = count_solutions(a, 2); /* only care >1 */
    if (sols != 1) a[r][c]=saved; else removed++;
  }
  if (removed < blanks) fprintf(stderr, "Warning: hanya bisa menghapus %d dari %d.\n", removed, blanks);
}

int is_full(int a[N][N]) {
  for (int i=0;i<N;i++) for (int j=0;j<N;j++) if (a[i][j]==0) return 0;
  return 1;
}

int main(int argc, char **argv) {
  srand((unsigned)time(NULL));
  int blanks = 6; /* default */
  if (argc>=2) { int v = atoi(argv[1]); if (v>=0 && v<=16) blanks = v; }

  int board[N][N];
  copy_board(board, base_solved);
  randomize_solution(board);

  int solution[N][N];
  copy_board(solution, board);

  generate_puzzle(board, blanks);

  int given[N][N];
  for (int i=0;i<N;i++) for (int j=0;j<N;j++) given[i][j] = (board[i][j] != 0);

  char line[64];
  printf("4x4 Sudoku. Perintah: 'baris kolom nilai' (mis: 2 3 4), 'reveal' untuk solusi, 'q' keluar.\n\n");

  while (1) {
    print_board(board);
    if (is_full(board)) { printf("Selamat! Selesai.\n"); break; }
    printf("Masukkan: ");
    if (!fgets(line, sizeof(line), stdin)) break;
    int len = (int)strlen(line); while (len>0 && (line[len-1]=='\n' || line[len-1]=='\r')) { line[--len]=0; }

    if (strcmp(line,"q")==0 || strcmp(line,"quit")==0) { printf("Keluar.\n"); break; }
    if (strcmp(line,"reveal")==0 || strcmp(line,"r")==0) { printf("=== SOLUSI ===\n"); print_board(solution); break; }

    int br, bc, bv;
    int n = sscanf(line, "%d %d %d", &br, &bc, &bv);
    if (n!=3) { printf("Format salah. Contoh: 2 3 4\n"); continue; }
    if (br<1||br>4||bc<1||bc>4||bv<1||bv>4) { printf("Nilai harus 1..4.\n"); continue; }
    int r=br-1, c=bc-1, v=bv;
    if (given[r][c]) { printf("Posisi (%d,%d) adalah given. Tidak bisa diubah.\n", br, bc); continue; }
    if (solution[r][c] != v) { printf("Salah. Coba lagi.\n"); continue; }
    board[r][c]=v;
    printf("Terisi (%d,%d) = %d\n\n", br, bc, v);
  }

  return 0;
}
