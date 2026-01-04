#include "optimizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
** STRICT TSV TOKENIZER
**
** Rules:
**  - Fields separated by '\t'
**  - Records end at '\n' or '\r\n'
**  - No quoting
**  - No escaping
**  - No multiline fields
*/

int tokenize_tsv_line(const char *buf,
                      char ***out_cells,
                      int *out_count)
{
    char **cells = NULL;
    int count = 0;

    size_t cap = 64;
    size_t len = 0;
    char *field = malloc(cap);

    if (!field)
        return -1;

#define PUSH_CHAR(ch) do { \
    if (len + 1 >= cap) { \
        cap *= 2; \
        char *tmp = realloc(field, cap); \
        if (!tmp) goto alloc_fail; \
        field = tmp; \
    } \
    field[len++] = (ch); \
} while (0)

#define PUSH_FIELD() do { \
    field[len] = '\0'; \
    char *copy = strdup(field); \
    if (!copy) goto alloc_fail; \
    char **tmp = realloc(cells, sizeof(char *) * (count + 1)); \
    if (!tmp) { free(copy); goto alloc_fail; } \
    cells = tmp; \
    cells[count++] = copy; \
    len = 0; \
} while (0)

    for (size_t i = 0;; i++) {
        char c = buf[i];

        /* End of line */
        if (c == '\0' || c == '\n' || c == '\r') {
            PUSH_FIELD();
            break;
        }

        /* Field separator */
        if (c == '\t') {
            PUSH_FIELD();
            continue;
        }

        /* Illegal character: embedded newline */
        if (c == '\n' || c == '\r') {
            goto parse_error;
        }

        PUSH_CHAR(c);
    }

    free(field);
    *out_cells = cells;
    *out_count = count;
    return count;

parse_error:
alloc_fail:
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
    free(field);
    return -1;
}
