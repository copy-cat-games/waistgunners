#include "hud.h"

long display_score = 0;

void update_hud() {
    long difference = score - display_score;
    if (!difference || frames % 3) return;
    for (int c = 5; c >= 0; c--) {
        int increment = 2 << c;
        if (increment <= difference) {
            display_score += increment;
            return;
        }
    }
}

long get_display_score() {
    return display_score;
}