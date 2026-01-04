#include "optimizer.h"

static int tsv_open(t_parser *p, const char *path)
{
    p->fp = fopen(path, "r");
    if (!p->fp)
        return -1;
    return 0;
}

static t_parse_error tsv_read_header(t_parser *p,
                                     char ***out_columns,
                                     int *out_count)
{
    return read_delimited_record(p->fp,
                                 '\t',
                                 out_columns,
                                 out_count);
}

static t_parse_error tsv_read_row(t_parser *p,
                                  char ***out_cells,
                                  int *out_count)
{
    return read_delimited_record(p->fp,
                                 '\t',
                                 out_cells,
                                 out_count);
}

static void tsv_close(t_parser *p)
{
    if (p->fp) {
        fclose(p->fp);
        p->fp = NULL;
    }
}

void use_tsv_parser(t_parser *p)
{
    p->delimiter = '\t';
    p->fp = NULL;
    p->open = tsv_open;
    p->read_header = tsv_read_header;
    p->read_row = tsv_read_row;
    p->close = tsv_close;
}
