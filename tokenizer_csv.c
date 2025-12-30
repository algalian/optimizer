#include "optimizer.h"

/*
** Robust CSV tokenizer: supports
** - quoted fields
** - escaped quotes ("")
** - variable delimiter (, ; \t)
*/


int tokenize_csv_line(const char *line, char delimiter, char ***out_cells, int *out_count)
{
    enum { FIELD_START, IN_FIELD, IN_QUOTED, IN_QUOTED_ESC } state = FIELD_START;

    char **cells = NULL;
    int count = 0;

    size_t bufcap = 64;
    size_t bi = 0;
    char *buffer = malloc(bufcap);
    if (!buffer)
        return -1;

    #define PUSH(c) do { \
        if (bi + 1 >= bufcap) { \
            bufcap *= 2; \
            char *tmp = realloc(buffer, bufcap); \
            if (!tmp) { free(buffer); return -1; } \
            buffer = tmp; \
        } \
        buffer[bi++] = (c); \
    } while (0)

    /* Handle UTF-8 BOM (if present) */
    if ((unsigned char)line[0] == 0xEF &&
        (unsigned char)line[1] == 0xBB &&
        (unsigned char)line[2] == 0xBF)
        line += 3;

    for (int i = 0; ; i++) {
        char c = line[i];
        int end = (c == '\0' || c == '\n' || c == '\r');

        switch (state) {

        case FIELD_START:
            if (end) {
				// empty field at end
                cells = realloc(cells, sizeof(char*) * (count + 1));
                cells[count++] = strdup("");
                goto finish;
            }
            else if (c == '"') {
                state = IN_QUOTED;
            }
            else if (c == delimiter) {
                cells = realloc(cells, sizeof(char*) * (count + 1));
                cells[count++] = strdup("");
            }
            else {
                PUSH(c);
                state = IN_FIELD;
            }
            break;

        case IN_FIELD:
            if (end || c == delimiter) {
                PUSH('\0');
                cells = realloc(cells, sizeof(char*) * (count + 1));
                cells[count++] = strdup(buffer);
                bi = 0;
                state = FIELD_START;
            } else {
                PUSH(c);
            }
            break;

        case IN_QUOTED:
            if (end) {
				// unterminated quote → still accept
                PUSH('\0');
                cells = realloc(cells, sizeof(char*) * (count + 1));
                cells[count++] = strdup(buffer);
                goto finish;
            }
            else if (c == '"') {
                state = IN_QUOTED_ESC;
            }
            else {
                PUSH(c);
            }
            break;

        case IN_QUOTED_ESC:
            if (c == '"') {
                PUSH('"');
                state = IN_QUOTED;
            }
            else if (c == delimiter || end) {
                PUSH('\0');
                cells = realloc(cells, sizeof(char*) * (count + 1));
                cells[count++] = strdup(buffer);
                bi = 0;
                state = FIELD_START;
            }
            else {
				// unexpected char, treat as raw
                PUSH(c);
                state = IN_FIELD;
            }
            break;
        }

        if (end)
            break;
    }

finish:
    *out_cells = cells;
    *out_count = count;
    free(buffer);
    return count;
}
