#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>

#define N 4
#define BLOCK 2
#define TOP 10
#define MAX_LINE 512
#define MAX_ENTRIES 1000

extern const int base_solved[N][N];

/* board operations */
void copy_board(int dst[N][N], const int src[N][N]);
void print_board(int a[N][N], double elapsed_seconds, int score);
int is_valid(int a[N][N], int r, int c, int v);
int count_solutions(int a[N][N], int limit);
void randomize_solution(int a[N][N]);
void generate_puzzle(int a[N][N], int blanks);

#endif /* BOARD_H */
