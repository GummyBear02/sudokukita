#include "leaderboard.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* safe name */
void safe_name(char *s) {
    for (int i=0;s[i];i++) if (s[i]==',') s[i]='_';
}

void save_score(const char *name,int score,double time_used) {
    FILE *fp=fopen("scores.csv","a");
    if (!fp) return;
    time_t t=time(NULL);
    struct tm *tm=localtime(&t);
    char date[64];
    strftime(date,sizeof(date),"%Y-%m-%d %H:%M:%S",tm);
    fprintf(fp,"%s,%d,%.2f,%s\n",name,score,time_used,date);
    fclose(fp);
}

/* cmp for qsort */
static int cmp(const void *A, const void *B) {
    const Entry *a = (const Entry *)A;
    const Entry *b = (const Entry *)B;
    if (a->score != b->score) return b->score - a->score;
    if (a->time_s < b->time_s) return -1;
    if (a->time_s > b->time_s) return 1;
    return strcmp(a->name, b->name);
}

/* case-insensitive substring */
static int contains_ci(const char *haystack, const char *needle) {
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

    while (fgets(line, sizeof(line), fp)) {
        int L = strlen(line);
        while (L > 0 && (line[--L] == '\n' || line[L] == '\r')) line[L]=0;
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
        } else arr[cnt].date[0]=0;

        cnt++;
        if (cnt >= MAX_ENTRIES) break;
    }
    fclose(fp);

    if (cnt == 0) { printf("Belum ada skor.\n"); return; }

    qsort(arr, cnt, sizeof(Entry), cmp);

    char q[128];

    while (1) {
        int show = cnt < TOP ? cnt : TOP;
        printf("\n===== TOP %d LEADERBOARD =====\n", show);
        printf("Rank | %-20s | Score | Time | Date\n", "Name");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < show; ++i) {
            printf("%4d | %-20s | %5d | %5.1f | %s\n",
                   i+1, arr[i].name, arr[i].score, arr[i].time_s, arr[i].date);
        }
        printf("\n");

        printf("Kosongkan dan Enter Untuk Ke Menu\n");
        printf("Cari Nama : ");
        if (!fgets(q, sizeof(q), stdin)) return;
        q[strcspn(q, "\n")]=0;

        if (q[0] == '\0') return;

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
            continue;
        }

        qsort(filtered, m, sizeof(Entry), cmp);

        printf("\n===== SEARCH RESULTS for \"%s\" (%d) =====\n", q, m);
        printf("Rank | %-20s | Score | Time | Date\n", "Name");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < m; ++i) {
            printf("%4d | %-20s | %5d | %5.1f | %s\n",
                   i+1, filtered[i].name, filtered[i].score, filtered[i].time_s, filtered[i].date);
        }
        printf("\n");
    }
}
