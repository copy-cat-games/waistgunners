#include "event.h"

ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_TIMER* timer;

void init_event_and_queue() {
    queue = al_create_event_queue();
    must_init(queue, "event queue");

    timer = al_create_timer(1.0 / 50.0);
    must_init(timer, "timer");

    al_register_event_source(queue, al_get_timer_event_source(timer));
}

void destroy_event_and_queue() {
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}