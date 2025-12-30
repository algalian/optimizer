#include "optimizer.h"

int load_channels_from_file(const char *filepath, char **fields, t_channel **list, t_globals *g)
{
    t_parser p = make_parser_for_file(filepath);


    if (p.open(&p, filepath) != 0) {
        fprintf(stderr, "Error: cannot open %s\n", filepath);
        return -1;
    }

    /* -------- READ HEADER -------- */
    char **header = NULL;
    int hcount = 0;
    if (p.read_header(&p, &header, &hcount) != 0) {
        fprintf(stderr, "Error: cannot read header.\n");
        p.close(&p);
        return -1;
    }

    /* -------- MAP COLUMNS -------- */
    t_colmap map;
    if (build_colmap(&map, header, hcount, fields) != 0) {
        fprintf(stderr, "Error: required fields not found in header.\n");
        free_cells(header, hcount);
        p.close(&p);
        return -1;
    }
    free_cells(header, hcount);

    /* -------- READ DATA ROWS -------- */
    char **cells = NULL;
    int ccount = 0;

    while (p.read_row(&p, &cells, &ccount)) 
    {
        t_channel *node = new_node();
        if (!node) 
            break;
        node->name    = strdup(cells[map.name]);
        node->a       = strtold(cells[map.a], NULL);
        node->b       = strtold(cells[map.b], NULL);
        node->c       = strtold(cells[map.c], NULL);
        node->cpm     = strtold(cells[map.cpm], NULL);
        append_node(list, node);
        free_cells(cells, ccount);
    }

    p.close(&p);
    return 0;
}


