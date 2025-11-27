/*
* TODO:
* 1. tutor main di awal
* 2. kurangin 6  =
* 3. majuin 8
* 4. Searching
* 5. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 4
#define BLOCK 2
#define MAX_LINE 512
#define MAX_ENTRIES 1000
#define TOP 10


/* -------------------- BASE SOLUTION -------------------- */
const int base_solved[N][N] = {
    {1,2,3,4},
    {3,4,1,2},
    {2,1,4,3},
    {4,3,2,1}
};

/* -------------------- UTILS -------------------- */
void copy_board(int dst[N][N], const int src[N][N]) {
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++)
            dst[i][j]=src[i][j];
}

int calculate_score(double elapsed_seconds) {
    int base_score = 1000;
    int time_penalty = (int)(elapsed_seconds * 2); 
    return base_score - time_penalty;
    }

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

/* -------------------- PRINT BOARD -------------------- */
void print_board(int a[N][N], double elapsed_seconds, int score) {
    int minutes = (int)(elapsed_seconds) / 60;
    int seconds = (int)(elapsed_seconds) % 60;

    printf("============================\n");
    printf("         SUDOKU 4x4\n");
    printf("============================\n");
    printf(" Time: %02d:%02d    Score: %3d\n", minutes, seconds, score);
    printf("============================\n\n");

    printf("        1  2   3  4\n");
    printf("      +------+------+\n");

    for (int i = 0; i < N; i++) {
        printf("    %d |", i + 1);

        for (int j = 0; j < N; j++) {
            if (a[i][j] == 0) printf(" .");
            else printf(" %d", a[i][j]);

            if (j != N - 1) printf(" ");

            if (j == 1) printf("|");
        }

        printf(" |\n");

        if (i == 1) printf("      +------+------+\n");
    }

    printf("      +------+------+\n");
    fflush(stdout);
}

/* -------------------- VALIDATION -------------------- */
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

/* -------------------- SOLVER (COUNT SOLUTIONS) -------------------- */

int solve_count_rec(int a[N][N], int limit, int *count) {
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

/* -------------------- TRANSFORMATIONS -------------------- */
void swap_rows(int a[N][N], int r1,int r2) {
    for (int c=0;c<N;c++){
        int t=a[r1][c]; a[r1][c]=a[r2][c]; a[r2][c]=t;
    }
}
void swap_cols(int a[N][N], int c1,int c2) {
    for (int r=0;r<N;r++){
        int t=a[r][c1]; a[r][c1]=a[r][c2]; a[r][c2]=t;
    }
}
void transpose(int a[N][N]) {
    for (int i=0;i<N;i++)
        for (int j=i+1;j<N;j++) {
            int t=a[i][j]; a[i][j]=a[j][i]; a[j][i]=t;
        }
}

void randomize_solution(int a[N][N]) {
    for (int it=0;it<50;it++) {
        int t=rand()%4;
        if (t==0) swap_rows(a, rand()%4, rand()%4);
        else if (t==1) swap_cols(a, rand()%4, rand()%4);
        else transpose(a);
    }
}

/* -------------------- PUZZLE GENERATION -------------------- */
void generate_puzzle(int a[N][N], int blanks) {
    int pos[16];
    for (int i=0;i<16;i++) pos[i]=i;

    for (int i=15;i>0;i--) {
        int j=rand()%(i+1);
        int t=pos[i]; pos[i]=pos[j]; pos[j]=t;
    }

    int removed=0;

    for (int k=0;k<16 && removed<blanks;k++) {
        int p=pos[k], r=p/4, c=p%4;
        if (a[r][c]==0) continue;

        int saved=a[r][c];
        a[r][c]=0;

        if (count_solutions(a,2)!=1)
            a[r][c]=saved;
        else removed++;
    }
}

/* -------------------- SCORE STORAGE -------------------- */
void safe_name(char *s) {
    for (int i=0;s[i];i++)
        if (s[i]==',') s[i]='_';
}

void save_score(const char *name,int score,double time_match) {
    FILE *fp=fopen("scores.csv","a");
    if (!fp) return;

    time_t t=time(NULL);
    struct tm *tm=localtime(&t);
    char date[64];
    strftime(date,sizeof(date),"%Y-%m-%d %H:%M:%S",tm);

    fprintf(fp,"%s,%d,%.2f,%s\n",name,score,time_match,date);
    fclose(fp);
}

int clamp(int x){ return x<0?0:x; }

/* -------------------- LEADERBOARD -------------------- */
typedef struct {
    char name[128];
    int score;
    double time_s;
    char date[64];
} Entry;

int cmp(const void *A, const void *B) {
    const Entry *a = (const Entry *)A;
    const Entry *b = (const Entry *)B;
    if (a->score != b->score) return b->score - a->score; /* desc by score */
    if (a->time_s < b->time_s) return -1;                 /* asc by time */
    if (a->time_s > b->time_s) return 1;
    return strcmp(a->name, b->name);                      /* tie-break by name */
}

/* case-insensitive substring check */
int contains_ci(const char *haystack, const char *needle) {
    if (!needle || !*needle) return 1;
    size_t hn = strlen(haystack), nn = strlen(needle);
    if (nn > hn) return 0;
    for (size_t i = 0; i + nn <= hn; ++i) {
        size_t k = 0;
        for (; k < nn; ++k) {
            char a = haystack[i+k], b = needle[k];
            if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
            if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
            if (a != b) break;
        }
        if (k == nn) return 1;
    }
    return 0;
}
void show_leaderboard(void) {
    FILE *fp = fopen("scores.csv", "r");
    if (!fp) {
        printf("Belum ada skor.\n");
        return;
    }

    Entry arr[MAX_ENTRIES];
    int cnt = 0;
    char line[MAX_LINE];

    /* baca semua skor */
    while (fgets(line, sizeof(line), fp)) {
        int L = strlen(line);
        while (L > 0 && (line[L-1] == '\n' || line[L-1] == '\r')) line[--L] = 0;
        if (L == 0) continue;

        char tmp[MAX_LINE];
        strncpy(tmp, line, sizeof(tmp)-1);
        tmp[sizeof(tmp)-1] = 0;

        char *name    = strtok(tmp, ",");
        char *score_s = strtok(NULL, ",");
        char *time_s  = strtok(NULL, ",");
        char *date    = strtok(NULL, "");

        if (!name || !score_s || !time_s) continue;

        strncpy(arr[cnt].name, name, sizeof(arr[cnt].name)-1);
        arr[cnt].name[sizeof(arr[cnt].name)-1] = 0;
        arr[cnt].score  = atoi(score_s);
        arr[cnt].time_s = atof(time_s);
        if (date) {
            strncpy(arr[cnt].date, date, sizeof(arr[cnt].date)-1);
            arr[cnt].date[sizeof(arr[cnt].date)-1] = 0;
        } else {
            arr[cnt].date[0] = 0;
        }

        cnt++;
        if (cnt >= MAX_ENTRIES) break;
    }
    fclose(fp);

    if (cnt == 0) {
        printf("Belum ada skor.\n");
        return;
    }

    /* sort sekali saja */
    qsort(arr, cnt, sizeof(Entry), cmp);

    char q[128];

    while (1) {
        /* tampilkan Top 10 dulu */
        int show = cnt < TOP ? cnt : TOP;
        printf("\n===== TOP %d LEADERBOARD =====\n", show);
        printf("Rank | %-20s | Score | Time | Date\n", "Name");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < show; ++i) {
            printf("%4d | %-20s | %5d | %5.1f | %s\n",
                   i+1, arr[i].name, arr[i].score, arr[i].time_s, arr[i].date);
        }
        printf("\n");

        /* prompt search berulang */
        printf("Kosongkan dan Enter Untuk Ke Menu\n");
        printf("Cari Nama : ");
        if (!fgets(q, sizeof(q), stdin)) return;
        q[strcspn(q, "\n")] = 0;   /* trim newline */

        if (q[0] == '\0') {
            /* user langsung Enter → keluar dari leaderboard */
            return;
        }

        /* filter berdasarkan query */
        Entry filtered[MAX_ENTRIES];
        int m = 0;
        for (int i = 0; i < cnt; ++i) {
            if (contains_ci(arr[i].name, q)) {
                filtered[m++] = arr[i];
                if (m >= MAX_ENTRIES) break;
            }
        }

        if (m == 0) {
            printf("Tidak ada entri yang cocok untuk \"%s\".\n\n", q);
            continue;   /* balik ke atas loop dan tanya lagi */
        }

        /* kita bisa pakai cmp lagi, tapi arr sudah sorted; untuk aman, sort lagi */
        qsort(filtered, m, sizeof(Entry), cmp);

        printf("\n===== SEARCH RESULTS for \"%s\" (%d) =====\n", q, m);
        printf("Rank | %-20s | Score | Time | Date\n", "Name");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < m; ++i) {
            printf("%4d | %-20s | %5d | %5.1f | %s\n",
                   i+1, filtered[i].name, filtered[i].score, filtered[i].time_s, filtered[i].date);
        }
        printf("\n");
        /* habis tampil, loop lagi: top10 muncul lagi + bisa search lagi */
    }
}


/* -------------------- PLAY GAME -------------------- */
void play_sudoku() {
    char name[128];
    printf("Masukkan nama pemain: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("1. Poin awal 1000 dan timer dimulai setelah menekan enter\n");
    printf("2. Masukkan angka dari 1-4 untuk kotak yang kosong (.)\n");
    printf("3. Masukkan dengan format (r c v) \n");
    printf("   r = row (baris) \n");
    printf("   c = coloumn (kolom) \n");
    printf("   v = variabel (angka) \n");
    printf("4. Ketik 'r' untuk menunjukkan jawaban \n");
    printf("5. Ketik 'q' untuk kembali ke menu\n");     

    printf("Tekan Enter untuk mulai...");

    /* ---- REFRESH BOARD TIAP GAME ---- */
    int board[N][N];
    copy_board(board, base_solved);
    randomize_solution(board);

    int solution[N][N];
    copy_board(solution, board);

    generate_puzzle(board, 6);     

    /* mask angka awal */
    int given[N][N];
    for (int i=0;i<N;i++)
        for (int j=0;j<N;j++)
            given[i][j] = (board[i][j] != 0);

    /* game logic */
    clock_t start_time = clock();
    double elapsed_seconds = 0;
    int score = 0;

    /* initialize msg to empty so we don't print garbage the first time */
    char msg[100] = "";

    /* make sure input buffer is clean before waiting for Enter */
    
    /* wait for the user to press Enter (consume the whole line) */
    {
        char _tmp[64];
        fgets(_tmp, sizeof(_tmp), stdin);
    }

    /* ---- now start the timer properly AFTER user pressed Enter ---- */
    start_time = clock();

    
    while(1) {
        clear_screen();
        print_board(board, elapsed_seconds, score);
        if (msg[0]) { printf("%s\n\n",msg); msg[0]=0; }

        elapsed_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        score=clamp(1000 - (int)(elapsed_seconds*2));

        printf("\nPlayer: %s | Time: %.1fs | Score: %d\n",name,elapsed_seconds,score);
        printf("Input (r c v), 'r (reveal)', 'q (keluar)': ");

        char in[64];
        if (!fgets(in,sizeof(in),stdin)) break;
        in[strcspn(in,"\n")]=0;

        if (!strcmp(in,"q")) break;
        if (!strcmp(in,"r")) {
            print_board(solution, elapsed_seconds, score);
            printf("Tekan Enter...");
            getchar();
            break;
        }

        int br,bc,bv;
        if (sscanf(in,"%d %d %d",&br,&bc,&bv)!=3) {
            sprintf(msg,"Format salah.");
            continue;
        }
        if (br<1||br>4||bc<1||bc>4||bv<1||bv>4) {
            sprintf(msg,"Nilai tidak valid.");
            sprintf(msg,"Silahkan masukkan angka 1-4");
            continue;
        }
        int r=br-1, c=bc-1, v=bv;

        if (given[r][c]) {
            sprintf(msg,"Tidak boleh ubah angka awal.");
            continue;
        }

        if (solution[r][c]!=v) {
            sprintf(msg,"Salah.");
            continue;
        }

        board[r][c]=v;

        int full=1;
        for (int i=0;i<N;i++)
            for (int j=0;j<N;j++)
                if (board[i][j]==0) full=0;

        if (full) {
            double elapsed_total=(double)(clock()-start_time)/CLOCKS_PER_SEC;
            int final_score=clamp(1000-(int)(elapsed_total*2));

            clear_screen();
            print_board(board, elapsed_seconds, score);
            printf("\nSelesai!\n");
            printf("Waktu: %.2fs\n",elapsed_total);
            printf("Skor : %d\n",final_score);

            save_score(name,final_score,elapsed_total);

            printf("\nSkor disimpan!\nTekan Enter...");
            getchar();
            break;
        }
    }
}

/* -------------------- MAIN MENU -------------------- */

int main() {
    srand(time(NULL)^clock());
    double elapsed_seconds = 0;
    int score = 0;

    while (1) {
        clear_screen();
        printf("====== SUDOKU 4x4 ======\n");
        printf("1. Main Sudoku\n");
        printf("2. Lihat Leaderboard (Top %d)\n",TOP);
        printf("3. Keluar\n");
        printf("Pilihan: ");

        char in[64];
        fgets(in,sizeof(in),stdin);
        int op=atoi(in);

        if (op==1) play_sudoku();
        else if (op==2) {
            clear_screen();
            show_leaderboard();
        }
        else if (op==3) break;
    }
}
