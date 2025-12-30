#include "optimizer.h"

void free_cells(char **cells, int count)
{
    if (!cells) return;
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
}

t_parser make_parser_for_file(const char *filename)
{
    t_parser p;

    const char *dot = strrchr(filename, '.');
    if (!dot) {
        use_csv_parser(&p);      // default
        return p;
    }

    if      (strcmp(dot, ".tsv") == 0) use_tsv_parser(&p);
    else if (strcmp(dot, ".csv") == 0) use_csv_parser(&p);
    else                                use_csv_parser(&p); // fallback

    return p;
}