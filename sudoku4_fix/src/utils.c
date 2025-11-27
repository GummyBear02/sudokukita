#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

/* flush stdin until newline (useful before waiting for Enter) */
void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

int clamp(int x){ return x<0?0:x; }

int calculate_score(double elapsed_seconds) {
    int base_score = 1000;
    int time_penalty = (int)(elapsed_seconds * 2);
    return clamp(base_score - time_penalty);
}
