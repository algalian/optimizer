#include "optimizer.h"

/*
** Strict CSV tokenizer
**
** Rules enforced:
**  - Quotes only allowed at start of field
**  - Quoted fields must end with delimiter or EOL
**  - Escaped quotes must be ""
**  - No garbage allowed after closing quote
**  - Unterminated quotes are reported
*/

int tokenize_csv_line(const char *buf,
                      char delimiter,
                      char ***out_cells,
                      int *out_count,
                      int *out_in_quotes)
{
    enum {
        FIELD_START,
        IN_FIELD,
        IN_QUOTED,
        AFTER_QUOTE
    } state = FIELD_START;

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
        int end = (c == '\0');

        if (!end && (c == '\n' || c == '\r'))
            end = 1;

        switch (state) {

        case FIELD_START:
            if (end) {
                PUSH_FIELD();
                goto done;
            }
            if (c == delimiter) {
                PUSH_FIELD();
            }
            else if (c == '"') {
                state = IN_QUOTED;
            }
            else {
                PUSH_CHAR(c);
                state = IN_FIELD;
            }
            break;

        case IN_FIELD:
            if (end || c == delimiter) {
                PUSH_FIELD();
                state = FIELD_START;
                if (end) goto done;
            }
            else if (c == '"') {
                /* Illegal quote inside unquoted field */
                goto parse_error;
            }
            else {
                PUSH_CHAR(c);
            }
            break;

        case IN_QUOTED:
            if (end) {
                *out_in_quotes = 1;
                goto done;
            }
            if (c == '"') {
                if (buf[i + 1] == '"') {
                    PUSH_CHAR('"');
                    i++; /* skip escaped quote */
                }
                else {
                    state = AFTER_QUOTE;
                }
            }
            else {
                PUSH_CHAR(c);
            }
            break;

        case AFTER_QUOTE:
            if (c == delimiter) {
                PUSH_FIELD();
                state = FIELD_START;
            }
            else if (end) {
                PUSH_FIELD();
                goto done;
            }
            else {
                /* Garbage after closing quote */
                goto parse_error;
            }
            break;
        }
    }

done:
    free(field);
    *out_cells = cells;
    *out_count = count;
    *out_in_quotes = (state == IN_QUOTED);
    return count;

parse_error:
alloc_fail:
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
    free(field);
    return -1;
}