#include <gint/std/stdlib.h>
#include <math.h>

#include "epidemic_engine.h"


int grid_get(const struct grid epidemic_grid, const int i, const int j)
{
    if (i < 0 || j < 0 || i >= epidemic_grid.width || j >= epidemic_grid.height) return 0;
    else return epidemic_grid.data[i + j * epidemic_grid.width];
}


bool can_become_infected(const struct grid epidemic_grid, const int mutations_selected[3], const int i, const int j)
{
    extern const uint8_t world[64][128];

    // In case of water, low or high temperature
    if (world[j][i] == 0 && mutations_selected[2] != 3 && mutations_selected[2] != 4 && mutations_selected[2] != 6) return false;
    if (world[j][i] == 1 && mutations_selected[1] != 1 && mutations_selected[1] != 5) return false;
    if (world[j][i] == 3 && mutations_selected[1] != 2 && mutations_selected[1] != 5) return false;
    
    // Test cases around
    return grid_get(epidemic_grid, i - 1, j) == 1 || grid_get(epidemic_grid, i + 1, j) == 1 || grid_get(epidemic_grid, i, j - 1) == 1 || grid_get(epidemic_grid, i, j + 1) == 1;
}


bool bernoulli(const int p)
{
    return (rand() % 101) < p;
}


void epidemic_simulation(struct game *current_game)
{
    extern const uint8_t world[64][128];

    srand(current_game->total_time);

    // Create a copy of the epidemic grid
    uint8_t *current_grid = calloc(current_game->grid.width * current_game->grid.height, sizeof(uint8_t));
    init_tab(current_game->grid.width * current_game->grid.height, current_grid, current_game->grid.data);

    int healed_rate =  0;
    if (current_game->research == current_game->limit) healed_rate = rand() % 6;

    // Make the epidemic grid evolove
    for (int i = 0; i < current_game->grid.width; i ++)
    {
        for (int j = 0; j < current_game->grid.height; j ++)
        {
            switch (current_game->grid.data[i + j * current_game->grid.width])
            {
                // Healthy
                case 0:

                    // Become infected
                    if (can_become_infected(current_game->grid, current_game->mutations_selected, i, j) && bernoulli(current_game->contagion))
                    {
                        current_grid[i + j * current_game->grid.width] = 1;
                        if (world[j][i])
                        {
                            current_game->humans[0] --;
                            current_game->humans[1] ++;
                        }
                    }
                    break;

                // Infected
                case 1:

                    // Become healed
                    if (bernoulli(healed_rate))
                    {
                        current_grid[i + j * current_game->grid.width] = 2;
                        if (world[j][i])
                        {
                            current_game->humans[1] --;
                            current_game->humans[2] ++;
                        }
                    }

                    // Become dead
                    else if (bernoulli(current_game->lethality))
                    {
                        current_grid[i + j * current_game->grid.width] = 3;
                        if (world[j][i])
                        {
                            current_game->humans[1] --;
                            current_game->humans[3] ++;
                        }
                    }
                    break;
            }

        }
    }
    init_tab(current_game->grid.width * current_game->grid.height, current_game->grid.data, current_grid);
    free(current_grid);

    for (int i = 0; i < 4; i ++)
    {
        if (current_game->humans[i] < 0) current_game->humans[i] = 0;
        else if (current_game->humans[i] > (current_game->grid.width * current_game->grid.height) - BLANK_CASES) current_game->humans[i] = (current_game->grid.width * current_game->grid.height) - BLANK_CASES;
    }
}


void init_tab(int x, uint8_t dest[x], uint8_t src[x])
{
    for (int i = 0; i < x; i++)
    {
        dest[i] = src[i];
    }
}