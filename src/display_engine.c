#include <gint/display.h>
#include <math.h>

#include "mutation_engine.h"
#include "display_engine.h"

void display_background(const int background)
{
    /**
     * 1 - map
     * 2 - navigation menu
     * 3 - info menu
     * 4 - grid
     * 5 - display message
     **/

    extern const bopti_image_t img_map;
    extern const bopti_image_t img_message;
    extern const bopti_image_t img_infos;
    extern const bopti_image_t img_grid;

    switch(background) {
        case 1:
            dimage(0, 0, &img_map);
            break;
        case 2:
            break;
        case 3:
            dimage(0, 0, &img_infos);
            break;
        case 4:
            dimage(0, 0, &img_grid);
            break;
        case 5:
            dimage(0, 0, &img_message);
            break;
    }
}


void display_foreground(const int background, const struct game *current_game, const int mutation_menu)
{
    extern const bopti_image_t img_mutations;
    extern const bopti_image_t img_planes;
    extern const uint8_t world[184][396];

    GUNUSED int length;

    switch (background)
    {
        case 1:

            // Disease propagation
            for (int i = 0; i < current_game->grid.width; i ++)
            {
                for (int j = 0; j < 48; j ++)
                {
                    if ((current_game->grid.data[i + j * current_game->grid.width] == 1 || current_game->grid.data[i + j * current_game->grid.width] == 3) && world[j][i] != 0) dpixel(i, j, C_BLACK);
                }
            }

            // Planes animations
            for (int i = 0; current_game->planes[i]; i++)
            {
                if (current_game->planes[i]->y + 8 < 184) dsubimage(current_game->planes[i]->x - 4, current_game->planes[i]->y - 4, &img_planes, 0, 8 * (current_game->planes[i]->direction - 1), 8, 8, DIMAGE_NONE);
            }

            // Research bar 
            int length = 73 * current_game->research / current_game->limit;
            dprint(30, 200, C_WHITE, "%d", current_game->dna);

            dline(51, 60, 51 + length, 60, C_BLACK);
            dline(51, 59, 51 + length, 59, C_BLACK);

            // Stats bar
            /*
            for (int i = 0; i < 4; i ++)
            {
                length = 63 * current_game->humans[i] / ((current_game->grid.width * current_game->grid.height) - BLANK_CASES);
                dline(61, i*8 + 31, 61 + length, i*8 + 31, C_BLACK);
                dline(61, i*8 + 32, 61 + length, i*8 + 32, C_BLACK);
            }
            */

            // Display if boost is activated
            if (current_game->boost) dprint(0, 0, C_WHITE, "+");

            break;

        case 2:
            drect(mutation_menu + 31 * (mutation_menu - 1), 0, mutation_menu + 31 * (mutation_menu) - 1, 7, C_INVERT);

            dprint(102, 37, C_BLACK, "%d", current_game->dna);
            
            length = 67 * current_game->contagion / 100;
            dline(57, 48, 57 + length, 48, C_BLACK);
            dline(57, 49, 57 + length, 49, C_BLACK);
            
            length = 67 * current_game->severity / 100;
            dline(57, 54, 57 + length, 54, C_BLACK);
            dline(57, 55, 57 + length, 55, C_BLACK);
            
            length = 67 * current_game->lethality / 100;
            dline(57, 60, 57 + length, 60, C_BLACK);
            dline(57, 61, 57 + length, 61, C_BLACK);

            if (current_game->mutations_selected[0]) dsubimage(7, 15, &img_mutations, 0, 44 * (current_game->mutations_selected[0] - 1), 44, 44, 0);
            if (current_game->mutations_selected[1]) dsubimage(37, 15, &img_mutations, 44, 44 * (current_game->mutations_selected[1] - 1), 44, 44, 0);
            if (current_game->mutations_selected[2]) dsubimage(67, 15, &img_mutations, 88, 44 * (current_game->mutations_selected[2] - 1), 44, 44, 0);
            break;
    }
}


void display_mutation(const int table[4][8], const struct cursor c, const int mutation_menu)
{
    extern const bopti_image_t img_mutations;
    extern bopti_image_t img_cursor;

    dclear(C_WHITE);
    display_background(4);
    switch(mutation_menu) {
        case 1:
            dprint(138,14,C_WHITE, "SYMPTOMES");
            break;
        case 2:
            dprint(138,14,C_WHITE, "CAPACITES");
            break;
        case 3:
            dprint(117,14,C_WHITE, "TRANSMISSION");
            break;
    }
    
    for (int i = 0 ; i < 4 ; i++)
    {
        for (int j = 0 ; j < 8; j++)
        {
            if (table[i][j]) dsubimage(j*44+22, i*44+46, &img_mutations, 44 * (mutation_menu - 1), 44 * (table[i][j] - 1), 44, 44, DIMAGE_NONE);
        }
    }
    dsubimage((44 * c.x)+22, (44 * c.y)+46, &img_cursor, 0, 44 * (c.display), 44, 44, DIMAGE_NONE);
    dupdate();

}


void display_mutation_buy(const struct cursor c, const int mutation_menu, const int table[4][8], const int button_selected, const struct game *current_game)
{
    extern const bopti_image_t img_mutations;
    const int id = table[c.y][c.x];

    struct mutation *mutation_data = get_mutation_data(mutation_menu, id);
    struct mutation *mutation_sel = get_mutation_data(mutation_menu, current_game->mutations_selected[mutation_menu - 1]);

    dclear(C_WHITE);
    
    display_background(3);
    dsubimage(28, 91, &img_mutations, 44 * (mutation_menu - 1), 44 * (id - 1), 44, 44, DIMAGE_NONE);

    dprint(105, 85, C_BLACK, "NOM : %s", mutation_data->name);
    if (!current_game->mutations_bought[mutation_menu - 1][id - 1]) dprint(150, 180, C_RGB(123, 8, 8), "COUT ADN : %d", mutation_data->dna);
    else dprint(195, 180, C_RGB(123, 8, 8), "ACHETEE");
    dprint(105, 105, C_BLACK, "CONTAGION : %d (%d)", mutation_data->contagion, mutation_data->contagion - mutation_sel->contagion);
    dprint(105, 125, C_BLACK, "SEVERITE  : %d (%d)", mutation_data->severity, mutation_data->severity - mutation_sel->severity);
    dprint(105, 145, C_BLACK, "LETALITE  : %d (%d)", mutation_data->lethality, mutation_data->lethality - mutation_sel->lethality);

    if (button_selected) {
        drect(294, 8, 298, 26, C_WHITE);
        drect(294, 30, 391, 26, C_WHITE);
    }
    else {
        drect(294, 36, 298, 51, C_WHITE);
        drect(294, 55, 391, 51, C_WHITE);
    }

    dupdate();
}


void display_mutation_description(const char *name, const char *description, const int mutation_menu, const int id)
{
    extern const bopti_image_t img_mutations;

    int decalage = 0;

    dclear(C_WHITE);

    display_background(3);
    dsubimage(3, 21, &img_mutations, 44 * (mutation_menu - 1), 44 * (id - 1), 44, 44, DIMAGE_NONE);
    dprint(47, 25, C_BLACK, name);
    
    for (int i = 0; i < 4; i ++)
    {
        dtext_opt(25, 33 + i * 7, C_BLACK, C_WHITE, 0 ,0, description + decalage, 16);
        
        int offset = 0;
        while (description[decalage + offset] != '\0') offset += 1;

        if (!offset) break;
        else if (offset > 16) decalage += 16;
        else decalage += offset;
    }
    dupdate();
}


void display_message(char *msg)
{
    int decalage = 0;

    dclear(C_WHITE);
    display_background(5);
    for (int i = 0; i < 5; i ++)
    {
        dtext_opt(190, 20 * i + 30, C_BLACK, C_WHITE, 0, 0, msg + decalage, 13);
        
        int offset = 0;
        while (msg[decalage + offset] != '\0') offset += 1;

        if (!offset) break;
        else if (offset > 13) decalage += 13;
        else decalage += offset;
    }
    dupdate();
}


