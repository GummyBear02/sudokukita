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

/* solver that counts solutions up to limit */
int solve_count_recursive(int board[N][N], int limit, int *count) {
  if (*count >= limit) return *count;

  int row = -1, col = -1;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      if (board[i][j] == 0) { row = i; col = j; break; }
    }
    if (row != -1) break;
  }

  if (row == -1) { 
    (*count)++; 
    return *count; 
  }

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

/**
 * Count the number of solutions to the Sudoku board, up to limit.
 *
 * This function works by making a temporary copy of the Sudoku board,
 * and then calling solve_count_recursive to do the actual counting.
 *
 * The solve_count_recursive function is a recursive function that tries
 * placing each valid value in each empty cell of the board, and then
 * recursively tries to fill in the rest of the board. If the board
 * is completely filled, it increments the solution count.
 *
 * The limit parameter is used to stop counting solutions once we've found
 * the desired number of solutions. This is useful if we only want to know
 * if there is one solution, or if there are multiple solutions.
 *
 * @param a The Sudoku board to count solutions for.
 * @param limit The maximum number of solutions to count.
 * @return The number of solutions found, up to limit.
 */
int count_solutions(int a[N][N], int limit) {
  int tmp[N][N];
  copy_board(tmp, a);

  int cnt = 0;
  solve_count_recursive(tmp, limit, &cnt);
  
  return cnt;
}

/**
 * Swap two rows of the Sudoku board.
 *
 * This function takes two row indices, r1 and r2, and swaps the
 * corresponding rows of the Sudoku board. This transformation does
 * not change the validity of the Sudoku solution.
 *
 * @param a The Sudoku board to modify.
 * @param r1 The index of the first row to swap.
 * @param r2 The index of the second row to swap.
 */
void swap_rows(int a[N][N], int r1, int r2) {
  for (int c = 0; c < N; c++) { /* loop over all columns */
    int t = a[r1][c]; /* save the value at row r1, col c */
    a[r1][c] = a[r2][c]; /* set the value at row r1, col c to the value at row r2, col c */
    a[r2][c] = t; /* set the value at row r2, col c to the saved value */
  }
}

/**
 * Swap two columns of the Sudoku board.
 *
 * This function takes two column indices, c1 and c2, and swaps the
 * corresponding columns of the Sudoku board. This transformation does
 * not change the validity of the Sudoku solution.
 *
 * @param a The Sudoku board to modify.
 * @param c1 The index of the first column to swap.
 * @param c2 The index of the second column to swap.
 */
void swap_cols(int a[N][N], int c1, int c2) {
  /* loop over all rows */
  for (int r = 0; r < N; r++) {
    /* save the value at row r, col c1 */
    int t = a[r][c1];
    /* set the value at row r, col c1 to the value at row r, col c2 */
    a[r][c1] = a[r][c2];
    /* set the value at row r, col c2 to the saved value */
    a[r][c2] = t;
  }
}

/**
 * Transpose a Sudoku board.
 *
 * This function takes a Sudoku board and transposes it.
 * Transposing a Sudoku board means swapping the rows and columns.
 * The resulting board is a valid Sudoku board, but it is not
 * necessarily a solution to the Sudoku puzzle.
 *
 * @param a The Sudoku board to transpose.
 */
void transpose_board(int a[N][N]) {
  /* loop over all rows and columns */
  for (int i = 0; i < N; i++) {
    for (int j = i+1; j < N; j++) { /* loop only over upper triangle */
      int t = a[i][j]; /* save the value at row i, col j */
      
      a[i][j] = a[j][i]; /* set the value at row i, col j to the value at row j, col i */
      a[j][i] = t; /* set the value at row j, col i to the saved value */
    }
  }
}

/**
 * Swap two blocks of rows in the Sudoku board.
 *
 * This function takes a Sudoku board and two block indices, b1 and b2,
 * and swaps the corresponding blocks of rows in the board. A block
 * of rows is a set of contiguous rows that are the same size
 * as the block size. The block size is defined as a constant
 * BLOCK.
 *
 * For example, if the block size is 2, then the block of rows
 * starting at index 0 consists of the rows at indices 0 and 1.
 * The block of rows starting at index 2 consists of the rows at
 * indices 2 and 3.
 *
 * @param a The Sudoku board to modify.
 * @param b1 The index of the first block of rows to swap.
 * @param b2 The index of the second block of rows to swap.
 */
void swap_row_blocks(int a[N][N], int b1, int b2) {
  /* loop over each row in the block */
  for (int i = 0 ; i < BLOCK; i++) {
    /* swap the row at index b1*BLOCK + i with the row at index b2*BLOCK + i */
    swap_rows(a, b1 * BLOCK + i, b2 * BLOCK + i);
  }
}

/**
 * Swap two blocks of columns in the Sudoku board.
 *
 * This function takes a Sudoku board and two block indices, b1 and b2,
 * and swaps the corresponding blocks of columns in the board. A block
 * of columns is a set of contiguous columns that are the same size
 * as the block size. The block size is defined as a constant
 * BLOCK.
 *
 * For example, if the block size is 2, then the block of columns
 * starting at index 0 consists of the columns at indices 0 and 1.
 * The block of columns starting at index 2 consists of the columns at
 * indices 2 and 3.
 *
 * @param a The Sudoku board to modify.
 * @param b1 The index of the first block of columns to swap.
 * @param b2 The index of the second block of columns to swap.
 */
void swap_col_blocks(int a[N][N], int b1, int b2) {
  /* loop over each column in the block */
  for (int i = 0; i < BLOCK; i++) {
    /* swap the column at index b1*BLOCK + i with the column at index b2*BLOCK + i */
    swap_cols(a, b1 * BLOCK + i, b2 * BLOCK + i);
  }
}

void randomize_solution(int a[N][N]) {
  for (int it=0; it<50; it++) {
    int t = rand()%5;

    if (t == 0) { 
      int band=rand()%BLOCK; 
      int r1 = band*BLOCK + rand() % BLOCK; 
      int r2 = band*BLOCK + rand() % BLOCK; 
      
      if (r1!=r2) { 
        swap_rows(a,r1,r2); 
      }
    }
    else if (t == 1) { 
      int band=rand()%BLOCK; 
      int c1 = band * BLOCK + rand() % BLOCK; 
      int c2 = band * BLOCK + rand() % BLOCK; 
      
      if (c1 != c2) {
        swap_cols(a,c1,c2); }
    }
    else if (t == 2) { 
      int b1 = rand() % BLOCK, b2 = rand() % BLOCK; 
      
      if (b1 != b2) {
        swap_row_blocks(a, b1, b2); 
      }
    }
    else if (t == 3) { 
      int b1 = rand() % BLOCK, b2 = rand() % BLOCK; 
      
      if (b1 != b2) {
        swap_col_blocks(a, b1, b2); 
      }
    }
    else { 
      if (rand() % 3 == 0) {
        transpose_board(a); }
    }
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

  // Judul program improvisasi
  // ? Judul awal
  printf("=========================\n");
  printf("|\tSUDOKU 4x4\t|\n");
  printf("=========================\n");
  // ? Petunjuk
  printf("Petunjuk bermain:\n");
  printf("1. Masukkan dengan format seperti berikut\n"); 
  printf("\n");
  printf("   ==> baris, kolom, nilai <==\n\n");
  printf("   Misalkan: 2 3 4\n");
  printf("   Baris 2, Kolom 3, dan Nilai 4\n");
  printf("\n");
  printf("2. Ketik 'reveal' untuk melihat solusi\n");
  printf("3. Ketik 'q' untuk keluar\n");
  printf("\n");
  // ? judul akhir
  printf("=== PERMAINAN DIMULAI ===\n");
  printf("\n");

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
    printf("\nTerisi (%d,%d) = %d\n\n", br, bc, v);
  }

  return 0;
}
