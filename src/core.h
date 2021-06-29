#ifndef _PLAGUE_CORE_H
#define _PLAGUE_CORE_H

#include <gint/defs/types.h>

// Duration for internal clock (ms)
#define ENGINE_TICK 50
#define CURSOR_TICK 110
#define DNA_ANIMATION_TICK 150

// Duration for DNA points and infectious model (ms)
#define LIMIT_TICK 10000

// Number of planes on screen
#define NB_PLANES 5

// Number of non-infectable cases
#define BLANK_CASES 5382

struct grid
{    
    // 64, 128
    int height, width;

    // Use grid.data[i + j * grid.width] instead of grid.data[i][j]
    uint8_t *data;
};

// game : all statistics of the current game
struct game
{
    // Disease parameters
    int contagion, severity, lethality;

    // DNA points
    int dna;

    // Mutations counts, mutations selected, mutations bought (symptoms; abilities; transmissions)
    int mutations_count[3];
    int mutations_selected[3];
    int mutations_bought[3][14];

    // Research data
    int research, limit;
    float priority;

    // Humans stats : healthy, infected, dead, healed
    int humans[4];

    // Time and boost (10 times faster)
    int time, total_time, boost;

    // Planes
    struct plane *planes[NB_PLANES + 1];

    // Grid for epidemologic model
    struct grid grid;
};

// plane : information about planes
struct plane
{
    // Plane's coordinates
    int x, y;

    // Plane's direction
    int direction;

    // Coordinates of the plane's destination
    int dest_x, dest_y;

    // Coordinates of the plane's departure
    int depa_x, depa_y;

    int is_infected;

};

//cursor : cursor information for mutations selection
struct cursor
{
    // Cursor's coordinates
    int x, y;

    // If the cursor should be displayed (0: display ; 1: hide)
    int display;
};

// get_inputs : detect and manage inputs
int get_inputs(const int background, int *mutation_menu, int *boost);

// next_frame : compute the plane's positions
int next_frame(struct game *current_game, int *dna_animation, int *vaccine);

// rtc_key : get the key with RTC system
int rtc_key(void);

// callback_timer : basic timer
int callback_tick(volatile int *tick);

// message : display a message
void message(char *msg);

#endif /* _PLAGUE_CORE_H */
