#include "cloud.h"
#include "graphics.h"

#define MINIMUM_DELAY 25
#define MAXIMUM_DELAY 75

VECTOR CLOUD_SIZES[] = {
    // each corresponds to a cloud type, above
    { .x = 32, .y = 19 },
    { .x = 61, .y = 42 },
};

CLOUD clouds[MAX_CLOUDS];

int cloud_delay;

void update_clouds() {
    // also handles generating the clouds

    for (int c = 0; c < MAX_CLOUDS; c++) {
        CLOUD* cloud = &clouds[c];
        if (!cloud->used) continue;

        cloud->position.y += cloud->speed;
        if (cloud->position.y > BUFFER_HEIGHT) {
            cloud->used = false;
        }
    }

    /*
     * to generate each cloud
     * - get a bowl of water
     * - boil off all the water to get some water vapour
     * - reduce the air pressure to condense all the vapour
     *
     * haha, just kidding
     *
     * - pick a random type, and find its corresponding size
     * - pick an x coordinate that is between -size.x and BUFFER_WIDTH
     * - generate a cloud at (x, -size.y)
     */

    if (cloud_delay) {
        cloud_delay--;
        return;
    }

    int type        = (int) between(0, CLOUD_TYPES_N);
    VECTOR size     = CLOUD_SIZES[type];
    VECTOR position = { .x = between(-size.x, BUFFER_WIDTH), .y = -size.y };

    CLOUD new_cloud = { .position = position, .type = type, .used = true, .speed = between(1, 2) };

    for (int c = 0; c < MAX_CLOUDS; c++) {
        if (clouds[c].used) continue;
        clouds[c] = new_cloud;
        break;
    }

    cloud_delay = (int) between(MINIMUM_DELAY, MAXIMUM_DELAY);
}