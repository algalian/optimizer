#include "optimizer.h"

void free_cells(char **cells, int count)
{
    if (!cells) return;
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
}