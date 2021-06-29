#ifndef _MUTATION_ENGINE_H
#define _MUTATION_ENGINE_H

#include "core.h"
#include "display_engine.h"

// Basic limit research
#define RESEARCH_LIMIT 200

// mutation_table : contain the map of the mutation available
struct mutation_table
{
    // Data of the map
    int data[4][8];
};

// mutation : data about a mutation
struct mutation
{
    // Data of the mutation : contagion, lethality, severity, DNA cost
    int contagion, severity, lethality, dna, changement;

    // Name of the mutation
    char *name, *description;
};

// get_mutation : return the mutation table to display
void get_mutation(const struct game *current_game, const int mutation_menu, int table[4][8]);

// manage_mutation : an independant sub-programm which allow to select and see the mutations.
void mutation_select(struct game *current_game, const int mutation_menu);

// mutation_buy : UI interface to buy mutations and see informations on them
int mutation_buy(struct game *current_game, const struct cursor c, const int mutation_menu, const int table[4][8]);

// update_disease : update the disease parameters
void update_disease(struct game *current_game);

// get_mutation_data : get the information from a mutation
struct mutation * get_mutation_data(const int mutation_menu, const int id);

// init_mat : copy src into dest (for int)
void init_mat(int x, int y, int dest[][x], int src[][x]);

#endif /* _MUTATION_ENGINE_H */