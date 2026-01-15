#include <errno.h>
#include <ctype.h>
#include "optimizer.h"


int parse_long_double(const char *s,
                      long double *out,
                      const char *col_name)
{
    char *end;
    long double val;

    if (!s || *s == '\0') {
        fprintf(stderr,
                "Error: empty numeric field (column '%s')\n",
                 col_name);
        return -1;
    }

    errno = 0;
    val = strtold(s, &end);

    if (errno == ERANGE) {
        fprintf(stderr,
                "Error: numeric overflow  (column '%s')\n",
                 col_name);
        return -1;
    }

    if (end == s) {
        fprintf(stderr,
                "Error: invalid numeric value '%s'  (column '%s')\n",
                s,  col_name);
        return -1;
    }

    /* Skip trailing whitespace */
    while (*end && isspace((unsigned char)*end))
        end++;

    if (*end != '\0') {
        fprintf(stderr,
                "Error: trailing garbage '%s'  (column '%s')\n",
                end,  col_name);
        return -1;
    }

    *out = val;
    return 0;
}
