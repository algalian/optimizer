#include "optimizer.h"


int find_col(char **columns, int col_count, const char *name)
{
    for (int i = 0; i < col_count; i++)
        if (strcmp(columns[i], name) == 0)
            return i;
    return -1;
}

int build_colmap(t_colmap *map, char **header, int hcount, char **fields)
{
    map->name    = find_col(header, hcount, fields[4]); //fix this from main
    map->a       = find_col(header, hcount, fields[0]);
    map->b       = find_col(header, hcount, fields[1]);
    map->c       = find_col(header, hcount, fields[2]);
    map->cpm     = find_col(header, hcount, fields[3]);
    map->min     = find_col(header, hcount, fields[8]);

    //printf("%i %i %i %i %i\n", map->name, map->a, map->b, map->c, map->cpm);
    /* validate all are found */
    if (map->name < 0 ||
        map->a    < 0 ||
        map->b    < 0 ||
        map->c    < 0 ||
        map->cpm  < 0 ||
        map->min < 0)
    {
        return -1;
    }

    return 0;
}
