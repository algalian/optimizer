#include "optimizer.h"
#include <errno.h>

/* forward declaration */
int parse_long_double(const char *s,
                      long double *out,
                      int row,
                      const char *col_name);

static int row_matches_header(char **row,int count, char **fields)
{
    int i;
    int j;

    i = 0;
    while(i < 3)
    {
        j = 0;
        while(row[j])
        {
            if(strcmp(row[j],fields[i]) == 0)
            {
                break;
            }
            j++;
        }
        if(j >= count)
        {
            //printf("field %s not found\n", fields[i]);
            return (0);
        }
        i++;
    }
    return(1);
}

int load_channels_from_file(const char *filepath,
                            char **fields,
                            t_channel **list,
                            t_globals *g)
{
    t_parser p;
    t_parse_error err;
    int j;

    *list = NULL;

    if (make_parser_for_file(filepath, &p) != 0) {
        fprintf(stderr,
                "Error: cannot determine file format for '%s'\n",
                filepath);
        return -1;
    }

    if (p.open(&p, filepath) != 0) {
        fprintf(stderr,
                "Error: cannot open '%s'\n",
                filepath);
        return -1;
    }

    /* -------- FIND HEADER -------- */
char **header = NULL;
int hcount = 0;
int row_num = 0;
/*printf("let it be known that the fields are\n");
j = 0;
while(fields[j])
{
    printf("%s\n", fields[j]);
    j++;
}
printf("these above\n");*/
j = 0;
while (1) 
{
    err = p.read_row(&p, &header, &hcount);
    row_num++;

    if (err == PARSE_EOF) {
        fprintf(stderr,
                "Error: header not found in '%s'\n",
                filepath);
        p.close(&p);
        return -1;
    }

    if (err != PARSE_OK) {
        fprintf(stderr,
                "Error: parse failure before header (row %d)\n",
                row_num);
        p.close(&p);
        return -1;
    }
    if (row_matches_header(header, hcount, fields) == 1)
    {
        /*printf("we found 'em at %i\n", j);
        int i = 0;
        while(header[i])
        {
            printf("%s\n", header[i]);
            i++;
        }*/
        break;
    }
    free_cells(header, hcount);
    header = NULL;
    hcount = 0;
    j++;
}





    /* -------- MAP COLUMNS -------- */
    t_colmap map;
    if (build_colmap(&map, header, hcount, fields) != 0) {
        fprintf(stderr,
                "Error: required columns not found in header\n");
        free_cells(header, hcount);
        p.close(&p);
        return -1;
    }

    free_cells(header, hcount);

    /* -------- READ DATA ROWS -------- */
    char **cells = NULL;
    int ccount = 0;
    row_num = 1;

    while (1) 
    {
        err = p.read_row(&p, &cells, &ccount);

        if (err == PARSE_EOF)
            break;

        if (err != PARSE_OK) {
            fprintf(stderr,
                    "Error: parse failure at data row %d\n",
                    row_num);
            goto fail;
        }

        if (ccount != hcount) {
            fprintf(stderr,
                    "Error: column count mismatch at row %d "
                    "(expected %d, got %d)\n",
                    row_num, hcount, ccount);
            goto fail;
        }

        t_channel *node = new_node();
        if (!node) {
            fprintf(stderr,
                    "Error: out of memory at row %d\n",
                    row_num);
            goto fail;
        }

        /* -------- STRING FIELD -------- */
        node->name = strdup(cells[map.name]);
        if (!node->name) 
        {
            fprintf(stderr, "Error: out of memory at row %d (column '%s')\n", row_num, fields[map.name]);
            free(node);
            goto fail;
        }
        if(node->name[0] == '\0')
        {
            break;
        }

        /* -------- NUMERIC FIELDS -------- */
        if (parse_long_double(cells[map.a], &node->a,
                               row_num, fields[0]) != 0 ||
            parse_long_double(cells[map.b], &node->b,
                               row_num, fields[1]) != 0 ||
            parse_long_double(cells[map.c], &node->c,
                               row_num, fields[2]) != 0 ||
            parse_long_double(cells[map.cpm], &node->cpm,
                               row_num, fields[3]) != 0) 
        {
            free(node->name);
            free(node);
            goto fail;
        }
        node->n = row_num;
        append_node(list, node);
        free_cells(cells, ccount);
        cells = NULL;
        ccount = 0;
        row_num++;
    }

    p.close(&p);
    return 0;

fail:
    free_cells(cells, ccount);
    free_channels(*list);
    *list = NULL;
    p.close(&p);
    return -1;
}
