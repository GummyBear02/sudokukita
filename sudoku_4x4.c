// TODO:
// 1. Launch in new window if requested. Switch to the corresponding branch.

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

void print_board(int a[N][N], double elapsed_seconds, int score) {
  /* 
   * Print timer and score information
   */
  int minutes = (int)(elapsed_seconds) / 60;
  int seconds = (int)(elapsed_seconds) % 60;
  printf("=========================\n");
  printf("|\tSUDOKU 4x4\t|\n");
  printf("=========================\n");
  printf("Time: %02d:%02d | Score: %d\n", minutes, seconds, score);
  printf("=========================\n\n");
  
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
    /* 
     * If we've reached the end of the board without finding any empty cells,
     * then we've found a valid solution. Increment the solution count.
     */
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

int count_solutions(int a[N][N], int limit) {
  int tmp[N][N];
  copy_board(tmp, a);

  int cnt = 0;
  solve_count_recursive(tmp, limit, &cnt);
  
  return cnt;
}

void swap_rows(int a[N][N], int r1, int r2) {
  for (int c = 0; c < N; c++) { /* loop over all columns */
    int t = a[r1][c]; /* save the value at row r1, col c */
    a[r1][c] = a[r2][c]; /* set the value at row r1, col c to the value at row r2, col c */
    a[r2][c] = t; /* set the value at row r2, col c to the saved value */
  }
}

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

void swap_row_blocks(int a[N][N], int b1, int b2) {
  /* loop over each row in the block */
  for (int i = 0 ; i < BLOCK; i++) {
    /* swap the row at index b1*BLOCK + i with the row at index b2*BLOCK + i */
    swap_rows(a, b1 * BLOCK + i, b2 * BLOCK + i);
  }
}

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

    /*
     * Transformation 0: swap two rows in the same block.
     * This transformation is done by generating two random row indices within the same block,
     * and swapping the corresponding rows of the Sudoku board.
     */
    if (t == 0) { 
      int band=rand()%BLOCK; 
      int r1 = band*BLOCK + rand() % BLOCK; 
      int r2 = band*BLOCK + rand() % BLOCK; 
      
      if (r1!=r2) { 
        swap_rows(a,r1,r2); 
      }
    }
    /*
     * Transformation 1: swap two columns in the same block.
     * This transformation is done by generating two random column indices within the same block,
     * and swapping the corresponding columns of the Sudoku board.
     */
    else if (t == 1) { 
      int band=rand()%BLOCK; 
      int c1 = band * BLOCK + rand() % BLOCK; 
      int c2 = band * BLOCK + rand() % BLOCK; 
      
      if (c1 != c2) {
        swap_cols(a,c1,c2); 
      }
    }
    /*
     * Transformation 2: swap two blocks of rows.
     * This transformation is done by generating two random block indices, and
     * swapping the corresponding blocks of rows in the Sudoku board.
     */
    else if (t == 2) { 
      int b1 = rand() % BLOCK, b2 = rand() % BLOCK; 
      
      if (b1 != b2) {
        swap_row_blocks(a, b1, b2); 
      }
    }
    /*
     * Transformation 3: swap two blocks of columns.
     * This transformation is done by generating two random block indices, and
     * swapping the corresponding blocks of columns in the Sudoku board.
     */
    else if (t == 3) { 
      int b1 = rand() % BLOCK, b2 = rand() % BLOCK; 
      
      if (b1 != b2) {
        swap_col_blocks(a, b1, b2); 
      }
    }
    /*
     * Transformation 4: transpose the Sudoku board.
     * This transformation is done by swapping the rows and columns of the Sudoku board.
     */
    else { 
      if (rand() % 3 == 0) {
        transpose_board(a); 
      }
    }
  }
}

void generate_puzzle(int a[N][N], int blanks) {
  /* initialize an array of positions to shuffle */
  int pos[16];

  /* initialize the array of positions */
  for ( int i = 0; i < 16; i++ ) {
    pos[i] = i;
  }

  /* shuffle the array of positions */
  for ( int i = 15; i > 0; i-- ) {
    int j = rand() % ( i + 1 );
    int t = pos[i];
    
    pos[i] = pos[j];
    pos[j] = t;
  }

  /* count the number of cells removed */
  int removed = 0;

  /* loop over the shuffled positions and remove cells while keeping a unique solution */
  for ( int k = 0; k < 16 && removed < blanks; k++ ) {
    int p = pos[k];
    int r = p / N, c = p % N;

    /* skip over empty cells */
    if ( a[r][c] == 0) {
      continue;
    }

    /* save the value of the cell before removing it */
    int saved = a[r][c];
    a[r][c]=0;
    
    /* count the number of solutions to the board after removing the cell */
    int sols = count_solutions(a, 2); /* only care >1 */
    /* if the number of solutions is not 1, then restore the cell */
    if ( sols != 1) {
      a[r][c]=saved; 
    } 
    /* if the number of solutions is 1, then increment the count of removed cells */
    else {
      removed++;
    }
  }

  /* if the number of removed cells is less than the number of blanks, then print a warning */
  if ( removed < blanks ) {
    fprintf(stderr, "WARNING: Hanya bisa menghapus %d dari %d.\n", removed, blanks);
  }
}

int is_full(int a[N][N]) {
  /* loop over all rows and columns */
  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      /* if any cell is empty, then the board is not full */
      if ( a[i][j] == 0 ) {
        return 0;
      }
    }
  }

  /* if all cells are filled, then the board is full */
  return 1;
}

/**
 * Calculate score based on elapsed time using time-based formula.
 * Base score decreases over time - faster completion = higher score.
 * @param elapsed_seconds Time taken in seconds
 * @return Calculated score
 */
int calculate_score(double elapsed_seconds) {
  int base_score = 1000;
  int time_penalty = (int)(elapsed_seconds * 2); // 2 points per second
  return base_score - time_penalty;
}

/**
 * Clear the console screen for Windows.
 */
void clear_screen() {
  system("clear");
}

/**
 * Launch Sudoku game in new window using PowerShell with fallback chain.
 * Tries pwsh first, then powershell, then cmd as final fallback.
 */
void launch_in_new_window(int argc, char **argv) {
  // Build command line arguments string
  char args[256] = "";
  if (argc > 1) {
    sprintf(args, " %s", argv[1]);
  }
  
  // Try PowerShell 7+ (pwsh) first
  if (system("where pwsh >nul 2>&1") == 0) {
    char command[512];
    sprintf(command, "start pwsh -Command \"sudoku.exe%s --in-window; Read-Host 'Tekan Enter untuk keluar'\"", args);
    system(command);
    return;
  }
  // Try Windows PowerShell 5.1
  else if (system("where powershell >nul 2>&1") == 0) {
    char command[512];
    sprintf(command, "start powershell -Command \"sudoku.exe%s --in-window; Read-Host 'Tekan Enter untuk keluar'\"", args);
    system(command);
    return;
  }
  
  // Final fallback to cmd
  char command[512];
  sprintf(command, "start cmd /k sudoku.exe%s --in-window", args);
}

int main(int argc, char **argv) {
  // Check if we should launch in new window (no --in-window flag)
  // int launch_new_window = 1;
  // for (int i = 1; i < argc; i++) {
  //   if (strcmp(argv[i], "--in-window") == 0) {
  //     launch_new_window = 0;
  //     break;
  //   }
  // }
  
  // Launch in new window if requested
  // if (launch_new_window) {
  //   launch_in_new_window(argc, argv);
  //   return 0;
  // }

  srand( (unsigned)time(NULL) );

  int blanks = 6; /* default */
  if ( argc >= 2 ) {
    int v = atoi(argv[1]);

    if ( v >= 0 && v <= 16 ) {
      blanks = v;
    }
  }

  int board[N][N];
  copy_board(board, base_solved);
  randomize_solution(board);

  int solution[N][N];
  copy_board(solution, board);

  generate_puzzle(board, blanks);

  int given[N][N];
  for ( int i = 0; i < N; i++) {
    for ( int j = 0; j < N; j++ ) {
      given[i][j] = (board[i][j] != 0);
    }
  }

  // Initialize timer
  clock_t start_time = clock();
  double elapsed_seconds = 0;
  int score = 0;

  char line[64];

  // Judul program improvisasi
  // ? Judul awal
  printf("=========================\n");
  printf("|\tSUDOKU 4x4\t|\n");
  printf("=========================\n");
  // ? Petunjuk
  printf("Petunjuk bermain:\n");
  printf("1. Masukan dengan format seperti berikut\n");
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
  
  // Wait for user to start the game
  printf("Tekan Enter untuk memulai permainan...");
  getchar();
  
  char message[256] = ""; // Store messages to display between refreshes

  while (1) {
    // Calculate elapsed time and score
    elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    score = calculate_score(elapsed_seconds);
    
// Clear screen and refresh display
    clear_screen();
    
    print_board(board, elapsed_seconds, score);
    
    // Display any pending messages
    if (strlen(message) > 0) {
      printf("%s\n", message);
      message[0] = '\0'; // Clear message after displaying
    }

    if (is_full(board)) {
      printf("Selamat! Selesai.\n");
      printf("Final Time: %.2f seconds | Final Score: %d\n", elapsed_seconds, score);
      break;
    }

    printf("\nMasukan: ");

    if (!fgets(line, sizeof(line), stdin)) {
      break;
    }

    int len = (int)strlen(line);
    while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r' )) {
      line[--len] = 0;
    }

if (strcmp(line,"q") == 0 || strcmp(line,"quit") == 0) {
      printf("\n\nKeluar.\n");
      break;
    }

if (strcmp(line,"reveal") == 0 || strcmp(line,"r") == 0) {
      printf("\n=== SOLUSI ===\n\n");
      print_board(solution, elapsed_seconds, score);
      break;
    }

    int br, bc, bv;
    int n = sscanf(line, "%d %d %d", &br, &bc, &bv);

    if (n != 3) {
      sprintf(message, "Format salah. Contoh: 2 3 4");
      continue;
    }

    if (br < 1 || br > 4 || bc < 1 || bc > 4 || bv < 1 || bv > 4) {
      sprintf(message, "Nilai harus 1 hingga 4");
      continue;
    }

int r = br - 1, c = bc - 1, v = bv;
    if (given[r][c]) {
      sprintf(message, "Posisi (%d,%d) adalah given. Tidak bisa diubah.", br, bc);
      continue;
    }

    if (solution[r][c] != v) {
      sprintf(message, "Salah. Coba lagi.");
      continue;
    }

board[r][c] = v;

    sprintf(message, "Terisi (%d,%d) = %d", br, bc, v);
  }

  return 0;
}
