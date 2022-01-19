/*
  Project name ......: Plague CG
  Version ...........: 1.3.9
  Last modification .: 5 July 2021

  code and assets provided with licence :
  GNU General Public Licence v3.0
*/

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>
#include <gint/defs/types.h>
#include <gint/std/stdlib.h>
#include <gint/gint.h>

#include "core.h"
#include "display_engine.h"
#include "mutation_engine.h"
#include "save.h"

const char *VERSION = "1.3.10";

// title_screen : display the title screen
static void title_screen(void);


// main_loop : display background, foreground and manage inputs
int main_loop(struct game *current_game);


int main(void)
{
    extern font_t font_plague;
    dfont(&font_plague);

    title_screen();

    // Game statistics
    struct plane plane_1 = {81, 49, 2, 308, 49, 81, 49, 0};
    struct plane plane_2 = {115, 49, 3, 115, 119, 115, 49, 0};
    struct plane plane_3 = {224, 119, 1, 224, 49, 224, 119, 0};
    struct plane plane_4 = {224, 141, 2, 337, 141, 224, 141, 0};
    struct plane plane_5 = {224, 119, 4, 115, 119, 224, 119, 0};


    struct game current_game =
    {
        .contagion = 0,
        .severity = 0,
        .lethality = 0,

        .dna = 0,

        .mutations_count = {0, 0, 0},
        .mutations_selected = {0, 0, 0},
        .mutations_bought = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        .research = 0,
        .limit = RESEARCH_LIMIT,
        .priority = 0,
        .humans = {0, 1, 0, 0},

        .time = 0, .total_time = 0, .boost = 0,

        .planes = {&plane_1, &plane_2, &plane_3, &plane_4, &plane_5, NULL},

        .grid = {184, 396, NULL},
    };

    // Allocate memory
    current_game.grid.data = calloc(current_game.grid.width * current_game.grid.height, sizeof(uint8_t));
    for (int i = 0; i < current_game.grid.width * current_game.grid.height; i ++) current_game.grid.data[i] = 0;

    current_game.grid.data[290 + 50 * current_game.grid.width] = 1;
    current_game.humans[0] = (current_game.grid.width * current_game.grid.height) - 1 - BLANK_CASES;

    gint_world_switch(GINT_CALL(read_save, (void *)&current_game));

    int to_save = main_loop(&current_game);

    if (to_save) gint_world_switch(GINT_CALL(write_save, (void *)&current_game));
    else
    {
        // Display stats at the end of the game
        int opt = GETKEY_DEFAULT & ~GETKEY_MOD_SHIFT & ~GETKEY_MOD_ALPHA & ~GETKEY_REP_ARROWS;
        getkey_opt(opt, NULL);

        gint_world_switch(GINT_CALL(delete_save));
    }

    // Free memory
    free(current_game.grid.data);

    return 1;
}


static void title_screen(void)
{
    extern bopti_image_t img_title;

    dclear(C_BLACK);

    dimage(0, 0, &img_title);
    dprint_opt(5, 200, C_WHITE, C_NONE, 0, 0, "VERSION %s", VERSION, -1);

    dupdate();
    getkey();
}


int main_loop(struct game *current_game)
{
    int background = 1, mutation_menu = 4;
    int end = 0, to_save = 1, vaccine = 0;

    static volatile int tick = 1;
    int t = timer_configure(TIMER_ANY, ENGINE_TICK*1000, GINT_CALL(callback_tick, &tick));
    if (t >= 0) timer_start(t);

    while (!end)
    {
        // Real-time clock system
        while (!tick) sleep();
        tick = 0;

        // Update the screen
        dclear(C_WHITE);
        display_background(background);
        display_foreground(background, current_game, mutation_menu);
        dupdate();

        // Compute the motion of planes, DNA points and infectious model
        to_save = next_frame(current_game, &vaccine);
        if (!to_save) return 0;
        
        // Get inputs from the keyboard and manage it
        background = get_inputs(background, &mutation_menu, &current_game->boost);

        // Special actions : quit and manage mutations
        if (background == -1) end = 1;
        if (background == 4)
        {
            mutation_select(current_game, mutation_menu);
            background = 2;
        }
    }

    if (t >= 0) timer_stop(t);
    return 1;
}

