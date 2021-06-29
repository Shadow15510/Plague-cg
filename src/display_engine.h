#ifndef _PLAGUE_DISPLAY_ENGINE_H
#define _PLAGUE_DISPLAY_ENGINE_H

#include "core.h"

// display_background : display the background
void display_background(const int background);

// display_foreground : display the foreground, planes, statistics
void display_foreground(const int background, const struct game *current_game, const int mutation_menu, const int dna_animation);

// display_mutation : display the mutation selection screen
void display_mutation(const int table[4][8], const struct cursor c, const int mutation_menu);

// mutation_selected : display the mutation's informations screen
void display_mutation_buy(const struct cursor c, const int mutation_menu, const int table[4][8], const int button_selected, const struct game *current_game);

// display_mutation_description : display the description of the selected mutation
void display_mutation_description(const char *name, const char *description, const int mutation_menu, const int id);

// output_error : display text and message background
void display_message(char *msg);

#endif /* _PLAGUE_DISPLAY_ENGINE_H */