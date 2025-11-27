#ifndef LEADERBOARD_H
#define LEADERBOARD_H

typedef struct {
    char name[128];
    int score;
    double time_s;
    char date[64];
} Entry;

void show_leaderboard(void);
void save_score(const char *name,int score,double time_used);

#endif /* LEADERBOARD_H */
