#include "optimizer.h"
#include <xlsxio_read.h>

typedef struct s_xlsx_ctx {
    xlsxioreader      reader;
    xlsxioreadersheet sheet;
} t_xlsx_ctx;

/* -------- open -------- */

static int xlsx_open(t_parser *p, const char *path)
{
    t_xlsx_ctx *ctx = calloc(1, sizeof(*ctx));
    if (!ctx)
        return -1;

    ctx->reader = xlsxioread_open(path);
    if (!ctx->reader) {
        free(ctx);
        return -1;
    }

    /* Open the first worksheet */
    ctx->sheet = xlsxioread_sheet_open(ctx->reader, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!ctx->sheet) {
        xlsxioread_close(ctx->reader);
        free(ctx);
        return -1;
    }

    p->fp = (FILE *)ctx;
    return 0;
}


/* -------- read_header -------- */

static t_parse_error xlsx_read_header(t_parser *p,
                                      char ***out_cells,
                                      int *out_count)
{
    (void)p;
    (void)out_cells;
    (void)out_count;
    return PARSE_EOF;
}

/* -------- read_row -------- */

static t_parse_error xlsx_read_row(t_parser *p,
                                   char ***out_cells,
                                   int *out_count)
{
    t_xlsx_ctx *ctx = (t_xlsx_ctx *)p->fp;

    if (!ctx || !ctx->sheet)
        return PARSE_EOF;

    /* Advance to next row */
    if (!xlsxioread_sheet_next_row(ctx->sheet))
        return PARSE_EOF;

    /* Row exists, but no cells yet */
    *out_cells = NULL;
    *out_count = 0;
    return PARSE_OK;
}

/* -------- close -------- */

static void xlsx_close(t_parser *p)
{
    if (!p->fp)
        return;

    t_xlsx_ctx *ctx = (t_xlsx_ctx *)p->fp;

    if (ctx->sheet)
        xlsxioread_sheet_close(ctx->sheet);

    if (ctx->reader)
        xlsxioread_close(ctx->reader);

    free(ctx);
    p->fp = NULL;
}


/* -------- public hook -------- */

void use_xlsx_parser(t_parser *p)
{
    p->fp = NULL;
    p->delimiter = 0; /* unused */
    p->open = xlsx_open;
    p->read_header = xlsx_read_header;
    p->read_row = xlsx_read_row;
    p->close = xlsx_close;
}