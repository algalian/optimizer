#include <errno.h>
#include <ctype.h>
#include "optimizer.h"


int parse_long_double(const char *s,
                      long double *out,
                      int row,
                      const char *col_name)
{
    char *end;
    long double val;

    if (!s || *s == '\0') {
        fprintf(stderr,
                "Error: empty numeric field at row %d (column '%s')\n",
                row, col_name);
        return -1;
    }

    errno = 0;
    val = strtold(s, &end);

    if (errno == ERANGE) {
        fprintf(stderr,
                "Error: numeric overflow at row %d (column '%s')\n",
                row, col_name);
        return -1;
    }

    if (end == s) {
        fprintf(stderr,
                "Error: invalid numeric value '%s' at row %d (column '%s')\n",
                s, row, col_name);
        return -1;
    }

    /* Skip trailing whitespace */
    while (*end && isspace((unsigned char)*end))
        end++;

    if (*end != '\0') {
        fprintf(stderr,
                "Error: trailing garbage '%s' at row %d (column '%s')\n",
                end, row, col_name);
        return -1;
    }

    *out = val;
    return 0;
}
