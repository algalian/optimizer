#include "optimizer.h"

static int csv_open(t_parser *p, const char *path)
{
    p->fp = fopen(path, "r");
    if (!p->fp)
        return -1;
    return 0;
}

static t_parse_error csv_read_header(t_parser *p,
                                     char ***out_columns,
                                     int *out_count)
{
    return read_delimited_record(p->fp,
                                 p->delimiter,
                                 out_columns,
                                 out_count);
}

static t_parse_error csv_read_row(t_parser *p,
                                  char ***out_cells,
                                  int *out_count)
{
    return read_delimited_record(p->fp,
                                 p->delimiter,
                                 out_cells,
                                 out_count);
}

static void csv_close(t_parser *p)
{
    if (p->fp) {
        fclose(p->fp);
        p->fp = NULL;
    }
}

void use_csv_parser(t_parser *p)
{
    p->delimiter = ',';
    p->fp = NULL;
    p->open = csv_open;
    p->read_header = csv_read_header;
    p->read_row = csv_read_row;
    p->close = csv_close;
}
