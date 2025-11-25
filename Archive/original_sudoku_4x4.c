/* sudoku4_play.c
   4x4 Sudoku generator + interactive player
   Compile: gcc -O2 sudoku4_play.c -o sudoku4_play
   Run:     ./sudoku4_play 6   (arg = jumlah blanks, default 6)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 4
#define BLOCK 2

/* a valid solved 4x4 board (2x2 blocks) */
int base_solved[N][N] = {
  {1,2,3,4},
  {3,4,1,2},
  {2,1,4,3},
  {4,3,2,1}
};

void copy_board(int dst[N][N], int src[N][N]) {
  for (int i=0;i<N;i++) for (int j=0;j<N;j++) dst[i][j]=src[i][j];
}

void print_board(int a[N][N]) {
  puts("   1 2   3 4");
  puts("  +-----+-----+");
  for (int i=0;i<N;i++) {
    printf("%d |", i+1);
    for (int j=0;j<N;j++) {
      if (a[i][j]==0) putchar('.');
      else putchar('0' + a[i][j]);
      if (j==1) putchar('|');
      else if (j!=N-1) putchar(' ');
    }
    printf("|\n");
    if (i==1) puts("  +-----+-----+");
  }
  puts("  +-----+-----+");
}

/* validity check for placing v at r,c */
int is_valid(int a[N][N], int r, int c, int v) {
  for (int i=0;i<N;i++) {
    if (a[r][i]==v) return 0;
    if (a[i][c]==v) return 0;
  }
  int br = (r / BLOCK) * BLOCK;
  int bc = (c / BLOCK) * BLOCK;
  for (int i=0;i<BLOCK;i++) for (int j=0;j<BLOCK;j++)
    if (a[br+i][bc+j] == v) return 0;
  return 1;
}

/* solver that counts solutions up to limit */
int solve_count_rec(int a[N][N], int limit, int *count) {
  if (*count >= limit) return *count;
  int r=-1,c=-1;
  for (int i=0;i<N;i++){
    for (int j=0;j<N;j++){
      if (a[i][j]==0) { r=i; c=j; goto found; }
    }
  }
found:
  if (r==-1) { (*count)++; return *count; }

  for (int v=1; v<=N; v++) {
    if (is_valid(a,r,c,v)) {
      a[r][c]=v;
      solve_count_rec(a, limit, count);
      a[r][c]=0;
      if (*count >= limit) return *count;
    }
  }
  return *count;
}

int count_solutions(int a[N][N], int limit) {
  int tmp[N][N];
  copy_board(tmp, a);
  int cnt = 0;
  solve_count_rec(tmp, limit, &cnt);
  return cnt;
}

/* simple transformations that preserve solution validity */
void swap_rows(int a[N][N], int r1, int r2) {
  for (int c=0;c<N;c++) { int t=a[r1][c]; a[r1][c]=a[r2][c]; a[r2][c]=t; }
}
void swap_cols(int a[N][N], int c1, int c2) {
  for (int r=0;r<N;r++) { int t=a[r][c1]; a[r][c1]=a[r][c2]; a[r][c2]=t; }
}
void transpose_board(int a[N][N]) {
  for (int i=0;i<N;i++) for (int j=i+1;j<N;j++) { int t=a[i][j]; a[i][j]=a[j][i]; a[j][i]=t; }
}
void swap_row_blocks(int a[N][N], int b1, int b2) {
  for (int i=0;i<BLOCK;i++) swap_rows(a, b1*BLOCK + i, b2*BLOCK + i);
}
void swap_col_blocks(int a[N][N], int b1, int b2) {
  for (int i=0;i<BLOCK;i++) swap_cols(a, b1*BLOCK + i, b2*BLOCK + i);
}

void randomize_solution(int a[N][N]) {
  for (int it=0; it<50; it++) {
    int t = rand()%5;
    if (t==0) { int band=rand()%BLOCK; int r1=band*BLOCK + rand()%BLOCK; int r2=band*BLOCK + rand()%BLOCK; if (r1!=r2) swap_rows(a,r1,r2); }
    else if (t==1) { int band=rand()%BLOCK; int c1=band*BLOCK + rand()%BLOCK; int c2=band*BLOCK + rand()%BLOCK; if (c1!=c2) swap_cols(a,c1,c2); }
    else if (t==2) { int b1=rand()%BLOCK, b2=rand()%BLOCK; if (b1!=b2) swap_row_blocks(a,b1,b2); }
    else if (t==3) { int b1=rand()%BLOCK, b2=rand()%BLOCK; if (b1!=b2) swap_col_blocks(a,b1,b2); }
    else { if (rand()%3==0) transpose_board(a); }
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
