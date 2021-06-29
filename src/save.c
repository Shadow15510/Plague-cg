#include <gint/bfile.h>
#include <gint/defs/types.h>
#include <gint/std/stdlib.h>

#include "save.h"


// Name of the savefile
static const uint16_t *filename = u"\\\\fls0\\Plague.sav";


void read_save(struct game *current_game)
{
    struct BFile_FileInfo fileInfo;
    int fd, handle;
    uint16_t foundpath[30];

    // Sizes of data and init
    const int planes_size = sizeof(*current_game->planes) * (NB_PLANES + 1);
    struct plane *new_planes[NB_PLANES + 1];
    for (int i = 0; i < NB_PLANES; i ++)
    {
        new_planes[i] = current_game->planes[i];
    }
    
    const int data_size = current_game->grid.width * current_game->grid.height;
    uint8_t *data = current_game->grid.data;
    
    // Check if the savefile exists
    char checkfile = BFile_FindFirst(filename, &handle, foundpath, &fileInfo);
    BFile_FindClose(handle);

    // If file doesn't exists
    if (checkfile == -1)
    {
        int size = sizeof(struct game) + planes_size + data_size;
        BFile_Create(filename, BFile_File, &size);
    }

    // Loading game data
    else 
    {
        fd = BFile_Open(filename, BFile_ReadOnly);
        BFile_Read(fd, current_game, sizeof(struct game), 0);
        
        // Overwritten struct plane* planes
        for (int i = 0; i < NB_PLANES; i ++)
        {
            current_game->planes[i] = new_planes[i];
        }

        // Loa
        for (int i = 0; i < NB_PLANES; i ++)
        {
            BFile_Read(fd, current_game->planes[i], sizeof(struct plane), -1);
        }
        BFile_Read(fd, data, data_size, -1);
        BFile_Close(fd);
        
        current_game->grid.data = data;
    }
}


void write_save(const struct game *current_game)
{
    // Remove the old savefile
    BFile_Remove(filename);

    // Create a new one
    const int data_size = current_game->grid.width * current_game->grid.height;
    int size = sizeof(struct game) + NB_PLANES * sizeof(struct plane) + data_size;
    BFile_Create(filename, BFile_File, &size);

    int fd = BFile_Open(filename, BFile_WriteOnly);
    
    // Write data
    BFile_Write(fd, current_game, sizeof(struct game));
    for (int i = 0; i < NB_PLANES ; i ++)
    {
        BFile_Write(fd, current_game->planes[i], sizeof(struct plane));
    }
    BFile_Write(fd, current_game->grid.data, data_size);
    
    // Close file
    BFile_Close(fd);
}


void delete_save(void)
{
    BFile_Remove(filename);
}