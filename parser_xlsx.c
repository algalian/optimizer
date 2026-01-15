#include "optimizer.h"
#include <xlsxio_read.h>
#include <xlsxio_write.h>

typedef struct s_xlsx_ctx {
    xlsxioreader      reader;
    xlsxioreadersheet sheet;
} t_xlsx_ctx;


static int xlsx_write_cell(t_parser *p, int row, int col, const char *value)
{
    return 0;
}

static int xlsx_save(t_parser *p)
{
    return 0;
}

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
    //printf("the sheet is %s\n", p->sheet_name);
    ctx->sheet = xlsxioread_sheet_open(ctx->reader, p->sheet_name, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!ctx->sheet) {
        fprintf(stderr,
        "Error: sheet '%s' not found\n",
        p->sheet_name ? p->sheet_name : "(null)");
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


/* -------- close -------- */

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

    char **cells = NULL;
    int count = 0;

    char *value;

    /* Read all cells in the row */
    while ((value = xlsxioread_sheet_next_cell(ctx->sheet)) != NULL) {

        char *cell = value ? strdup(value) : strdup("");
        if (!cell)
            goto alloc_fail;

        char **tmp = realloc(cells, sizeof(char *) * (count + 1));
        if (!tmp) {
            free(cell);
            goto alloc_fail;
        }

        cells = tmp;
        cells[count++] = cell;
    }

    *out_cells = cells;
    *out_count = count;
    return PARSE_OK;

alloc_fail:
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
    return PARSE_ALLOC_FAIL;
}

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
    p->write_cell = xlsx_write_cell;
    p->save = xlsx_save;
}