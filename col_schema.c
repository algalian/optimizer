#include "optimizer.h"


/* ------------------ Helpers ------------------ */

static int find_column_index(char **header, int hcount, const char *name)
{
    for (int i = 0; i < hcount; i++) {
        if (strcmp(header[i], name) == 0)
            return i;
    }
    return -1;
}

static int parse_ldouble(const char *s,
                         long double *out,
                         int row,
                         const char *col)
{
    char *end;
    long double val;

    if (!s || *s == '\0') {
        fprintf(stderr,
                "Error: empty numeric field at row %d (column '%s')\n",
                row, col);
        return -1;
    }

    errno = 0;
    val = strtold(s, &end);

    if (errno == ERANGE) {
        fprintf(stderr,
                "Error: numeric overflow at row %d (column '%s')\n",
                row, col);
        return -1;
    }

    if (end == s) {
        fprintf(stderr,
                "Error: invalid numeric value '%s' at row %d (column '%s')\n",
                s, row, col);
        return -1;
    }

    while (*end && isspace((unsigned char)*end))
        end++;

    if (*end != '\0') {
        fprintf(stderr,
                "Error: trailing garbage '%s' at row %d (column '%s')\n",
                end, row, col);
        return -1;
    }

    *out = val;
    return 0;
}

/* ------------------ Schema Loader ------------------ */

int load_channel_from_row(t_channel       *dst,
                          char           **cells,
                          int              row,
                          char           **header,
                          int              hcount,
                          const t_column_spec *schema)
{
    for (int i = 0; schema[i].header_name; i++) {

        int col = find_column_index(header, hcount,
                                    schema[i].header_name);

        if (col < 0) {
            if (schema[i].required) {
                fprintf(stderr,
                        "Error: missing required column '%s'\n",
                        schema[i].header_name);
                return -1;
            }
            continue;
        }

        void *field = (char *)dst + schema[i].offset;

        switch (schema[i].type) {

        case COL_STRING:
            *(char **)field = strdup(cells[col]);
            if (!*(char **)field) {
                fprintf(stderr,
                        "Error: out of memory at row %d (column '%s')\n",
                        row, schema[i].header_name);
                return -1;
            }
            break;

        case COL_LDOUBLE:
            if (parse_ldouble(cells[col],
                              (long double *)field,
                              row,
                              schema[i].header_name) != 0)
                return -1;
            break;
        }
    }
    return 0;
}
