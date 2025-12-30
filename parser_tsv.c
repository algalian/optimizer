#include "optimizer.h"

static int tsv_open(t_parser *p, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;
    p->ctx = fp;
    return 0;
}
static int tsv_read_header(t_parser *p, char ***out_columns, int *out_count)
{
    FILE *fp = (FILE *)p->ctx;
    char *line = get_next_line(fileno(fp));
    if (!line)
        return -1;

    p->delimiter = '\t';  // TSV always uses tabs

    tokenize_csv_line(line, p->delimiter, out_columns, out_count);
    free(line);
    return 0;
}

static int tsv_read_row(t_parser *p, char ***out_cells, int *out_count)
{
    FILE *fp = (FILE *)p->ctx;
    char *line = get_next_line(fileno(fp));
    if (!line)
        return 0;

    if (is_line_empty(line)) {
        free(line);
        return 0;
    }

    tokenize_csv_line(line, p->delimiter, out_cells, out_count);
    free(line);
    return 1;
}

static void tsv_close(t_parser *p)
{
    if (p->ctx)
        fclose((FILE *)p->ctx);
    p->ctx = NULL;
}

void use_tsv_parser(t_parser *p)
{
    p->ctx = NULL;
    p->delimiter = '\t';
    p->open = tsv_open;
    p->read_header = tsv_read_header;
    p->read_row = tsv_read_row;
    p->close = tsv_close;
}