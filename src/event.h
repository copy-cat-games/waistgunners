#include <allegro5/allegro.h>
#include "game_state.h"

#define FRAME_RATE 50.0;

extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_TIMER* timer;

void init_event_and_queue();
void destroy_event_and_queue();