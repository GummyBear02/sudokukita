#include "game.h"
#include "board.h"
#include "leaderboard.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/* game loop */
void play_sudoku() {
    char name[128];
    printf("Masukkan nama pemain: ");
    if (!fgets(name, sizeof(name), stdin)) return;
    name[strcspn(name, "\n")] = 0;

    printf("1. Poin awal 1000 dan timer dimulai setelah menekan enter\n");
    printf("2. Masukkan angka dari 1-4 untuk kotak yang kosong (.)\n");
    printf("3. Masukkan dengan format (r c v) \n");
    printf("   r = row (baris) \n");
    printf("   c = coloumn (kolom) \n");
    printf("   v = variabel (angka) \n");
    printf("4. Ketik 'r' untuk menunjukkan jawaban \n");
    printf("5. Ketik 'q' untuk kembali ke menu\n");

    printf("Tekan Enter untuk mulai...\n");

    /* create a fresh board every call */
    int board[N][N];
    copy_board(board, base_solved);
    randomize_solution(board);
    int solution[N][N];
    copy_board(solution, board);
    generate_puzzle(board, 6);

    int given[N][N];
    for (int i=0;i<N;i++) for (int j=0;j<N;j++) given[i][j] = (board[i][j] != 0);

    /* initialize message buffer to empty */
    char msg[128] = "";

    /* make sure no leftover input, then wait for Enter */
    flush_input();
    {
        char _tmp[64];
        fgets(_tmp, sizeof(_tmp), stdin);
    }

    /* start timer now */
    clock_t start_time = clock();

    while(1) {
        double elapsed_seconds = (double)(clock()-start_time)/CLOCKS_PER_SEC;
        int score = calculate_score(elapsed_seconds);

        clear_screen();
        print_board(board, elapsed_seconds, score);
        if (msg[0]) { printf("%s\n\n",msg); msg[0]=0; }

        printf("\nPlayer: %s | Time: %.1fs | Score: %d\n",name,elapsed_seconds,score);
        printf("Input (r c v), 'r (reveal)', 'q (keluar)': ");

        char in[64];
        if (!fgets(in,sizeof(in),stdin)) break;
        in[strcspn(in,"\n")] = 0;

        if (!strcmp(in,"q")) break;
        if (!strcmp(in,"r")) {
            clear_screen();
            print_board(solution, elapsed_seconds, score);
            printf("Tekan Enter...");
            fgets(in,sizeof(in),stdin);
            break;
        }

        int br,bc,bv;
        if (sscanf(in,"%d %d %d",&br,&bc,&bv)!=3) {
            snprintf(msg,sizeof(msg),"Format salah.");
            continue;
        }
        if (br<1||br>4||bc<1||bc>4||bv<1||bv>4) {
            snprintf(msg,sizeof(msg),"Silahkan masukkan angka 1-4");
            continue;
        }
        int r=br-1, c=bc-1, v=bv;

        if (given[r][c]) { snprintf(msg,sizeof(msg),"Tidak boleh ubah angka awal."); continue; }
        if (solution[r][c]!=v) { snprintf(msg,sizeof(msg),"Salah."); continue; }

        board[r][c]=v;

        int full=1;
        for (int i=0;i<N;i++) for (int j=0;j<N;j++) if (board[i][j]==0) full=0;

        if (full) {
            double elapsed_total=(double)(clock()-start_time)/CLOCKS_PER_SEC;
            int final_score=calculate_score(elapsed_total);

            clear_screen();
            print_board(board, elapsed_total, final_score);
            printf("\nSelesai!\n");
            printf("Waktu: %.2fs\n",elapsed_total);
            printf("Skor : %d\n",final_score);

            save_score(name,final_score,elapsed_total);

            printf("\nSkor disimpan!\nTekan Enter...");
            fgets(in,sizeof(in),stdin);
            break;
        }
    }
}
