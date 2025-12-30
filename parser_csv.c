#include "optimizer.h"

/* ========== CSV IMPLEMENTATION ========== */

static int csv_open(t_parser *p, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;
    p->ctx = fp;
    return 0;
}

/* Reads the CSV header (first non-empty line) */
static int csv_read_header(t_parser *p, char ***out_columns, int *out_count)
{
    FILE *fp = (FILE *)p->ctx;
    char *line = get_next_line(fileno(fp));
    if (!line)
        return -1;

    // Split by comma using strtok — basic version; we'll enhance later
    int count = 0;
    char **cols = NULL;

    char *token = strtok(line, ",\n\r");
    while (token) {
        cols = realloc(cols, sizeof(char*) * (count + 1));
        cols[count] = strdup(token);
        count++;
        token = strtok(NULL, ",\n\r");
    }

    *out_columns = cols;
    *out_count = count;
    free(line);
    return 0;
}

/* Reads next row: returns 1 if a row was found, 0 if EOF */
static int csv_read_row(t_parser *p, char ***out_cells, int *out_count)
{
    FILE *fp = (FILE *)p->ctx;
    char *line = get_next_line(fileno(fp));
    
    if (!line)
        return 0;

    if (is_line_empty(line)) {
        free(line);
        return 0;
    }

    int count = 0;
    char **cells = NULL;

    char *token = strtok(line, ",\n\r");
    while (token) {
        cells = realloc(cells, sizeof(char*) * (count + 1));
        cells[count] = strdup(token);
        count++;
        token = strtok(NULL, ",\n\r");
    }

    *out_cells = cells;
    *out_count = count;
    free(line);
    return 1;
}

static void csv_close(t_parser *p)
{
    if (p->ctx)
        fclose((FILE *)p->ctx);
    p->ctx = NULL;
}

/* Public function: configure parser to use CSV backend */
void use_csv_parser(t_parser *p)
{
    p->ctx = NULL;
    p->open = csv_open;
    p->read_header = csv_read_header;
    p->read_row = csv_read_row;
    p->close = csv_close;
}