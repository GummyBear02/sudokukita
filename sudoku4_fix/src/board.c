#include "board.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* base solution */
const int base_solved[N][N] = {
    {1,2,3,4},
    {3,4,1,2},
    {2,1,4,3},
    {4,3,2,1}
};

/* copy */
void copy_board(int dst[N][N], const int src[N][N]) {
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++)
            dst[i][j]=src[i][j];
}

/* print board with header */
void print_board(int a[N][N], double elapsed_seconds, int score) {
    int minutes = (int)(elapsed_seconds) / 60;
    int seconds = (int)(elapsed_seconds) % 60;

    printf("=================================\n");
    printf("         SUDOKU 4x4\n");
    printf("=================================\n");
    printf(" Time: %02d:%02d    Score: %3d\n", minutes, seconds, score);
    printf("=================================\n\n");

    printf("    1  2   3  4\n");
    printf("  +------+------+=\n");

    /* use consistent spacing */
    printf("    1  2   3  4\n");
    printf("  +------+------+\n");

    for (int i = 0; i < N; i++) {
        printf("%d |", i + 1);
        for (int j = 0; j < N; j++) {
            if (a[i][j] == 0) printf(" .");
            else printf(" %d", a[i][j]);
            if (j != N - 1) printf(" ");
            if (j == 1) printf("|");
        }
        printf(" |\n");
        if (i == 1) printf("  +------+------+\n");
    }
    printf("  +------+------+\n");
    fflush(stdout);
}

/* validation */
int is_valid(int a[N][N], int r, int c, int v) {
    for (int i=0;i<N;i++) {
        if (a[r][i]==v) return 0;
        if (a[i][c]==v) return 0;
    }
    int br=(r/BLOCK)*BLOCK, bc=(c/BLOCK)*BLOCK;
    for (int i=0;i<BLOCK;i++)
        for (int j=0;j<BLOCK;j++)
            if (a[br+i][bc+j]==v) return 0;
    return 1;
}

/* solver for counting solutions (cutoff) */
static int solve_count_rec(int a[N][N], int limit, int *count) {
    if (*count>=limit) return *count;
    int r=-1,c=-1;
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++)
            if (a[i][j]==0) { r=i; c=j; goto found; }
found:
    if (r==-1) { (*count)++; return *count; }
    for (int v=1;v<=N;v++) {
        if (is_valid(a,r,c,v)) {
            a[r][c]=v;
            solve_count_rec(a,limit,count);
            a[r][c]=0;
            if (*count>=limit) return *count;
        }
    }
    return *count;
}

int count_solutions(int a[N][N], int limit) {
    int tmp[N][N]; copy_board(tmp,a);
    int c=0;
    solve_count_rec(tmp,limit,&c);
    return c;
}

/* transforms */
void swap_rows(int a[N][N], int r1,int r2) {
    for (int c=0;c<N;c++){ int t=a[r1][c]; a[r1][c]=a[r2][c]; a[r2][c]=t; }
}
void swap_cols(int a[N][N], int c1,int c2) {
    for (int r=0;r<N;r++){ int t=a[r][c1]; a[r][c1]=a[r][c2]; a[r][c2]=t; }
}
void transpose(int a[N][N]) {
    for (int i=0;i<N;i++) for (int j=i+1;j<N;j++){ int t=a[i][j]; a[i][j]=a[j][i]; a[j][i]=t; }
}

void randomize_solution(int a[N][N]) {
    for (int it=0;it<50;it++) {
        int t=rand()%4;
        if (t==0) swap_rows(a, rand()%4, rand()%4);
        else if (t==1) swap_cols(a, rand()%4, rand()%4);
        else transpose(a);
    }
}

/* puzzle generator that ensures unique solution */
void generate_puzzle(int a[N][N], int blanks) {
    int pos[N*N];
    for (int i=0;i<N*N;i++) pos[i]=i;
    for (int i=N*N-1;i>0;i--) {
        int j=rand()%(i+1);
        int t=pos[i]; pos[i]=pos[j]; pos[j]=t;
    }
    int removed=0;
    for (int k=0;k<N*N && removed<blanks;k++) {
        int p=pos[k], r=p/N, c=p%N;
        if (a[r][c]==0) continue;
        int saved=a[r][c];
        a[r][c]=0;
        if (count_solutions(a,2)!=1) a[r][c]=saved;
        else removed++;
    }
}
