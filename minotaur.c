#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "game_engine.h"

Object MapProto = {
        .init = Map_init,
        .move = Map_move,
        .attack = Map_attack
};

int main(int argc, char *argv[])
{
        // simple way to setup the randomness
        srand(time(NULL));

        // make our map to work with
        Map *game = NEW(Map, "The Hall of the Minotaur");
        assert(game != NULL);

        printf("You enter the ");
        game->location->_(describe)(game->location);

        while (process_input(game)) {
        }

        return 0;
}
