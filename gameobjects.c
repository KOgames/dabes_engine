#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include "gameobjects.h"

int Thing_init(void *self) {
    check_mem(self);
    debug("blah");
    Thing *thing = (Thing *)self;
    thing->x = 270;
    thing->y = 0;
    return 1;

error:
    return 0;
}

void Thing_calc_physics(void *self) {
    check_mem(self);
    Thing *thing = (Thing *)self;
    if (thing->y >= 380) return;

    thing->velo += 1;
    thing->y += thing->velo;

error:
    return;
}

void Game_calc_physics(void *self) {
    check_mem(self);
    Game *game = (Game *)self;
    game->thing->_(calc_physics)(game->thing);

error:
    return;
}

void Game_destroy(void *self) {
    check_mem(self);
    Game *game = (Game *)self;
    game->thing->_(destroy)(game->thing);

    Object_destroy(self);
error:
    return;
}
