#ifndef _EPIDEMIC_ENGINE_H
#define _EPIDEMIC_ENGINE_H

#include "core.h"

// humans per case of the epidemic grid
#define HUMANS_PER_CASE 1000000

// can_become_infected : return 0 if the case (i, j) isn't infectable, 1 else
bool can_become_infected(const struct grid epidemic_grid, const int mutations_selected[3], const int i, const int j);

// bernoulli : simulate a random event
bool bernoulli(const int p);

// epidemic_simulation : simulate the propagation of the virus
void epidemic_simulation(struct game *current_game);

// init_tab : copy src into dest (for int)
void init_tab(int x, uint8_t dest[x], uint8_t src[x]);

#endif /* _EPIDEMIC_ENGINE_H */