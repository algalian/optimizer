#include "optimizer.h"

t_parse_error read_delimited_record(FILE *fp,
                                    char delimiter,
                                    char ***out_cells,
                                    int *out_count)
{
    char buffer[4096];

    *out_cells = NULL;
    *out_count = 0;

    if (!fgets(buffer, sizeof(buffer), fp))
        return PARSE_EOF;

    /* STRICT TSV */
    if (delimiter == '\t') {
        if (tokenize_tsv_line(buffer, out_cells, out_count) < 0)
            return PARSE_ALLOC_FAIL;
        return PARSE_OK;
    }

    /* CSV (multiline capable) */
    char *acc = NULL;
    size_t acc_len = 0;
    int in_quotes = 0;

    do {
        size_t chunk_len = strlen(buffer);
        char *tmp = realloc(acc, acc_len + chunk_len + 1);
        if (!tmp) {
            free(acc);
            return PARSE_ALLOC_FAIL;
        }

        acc = tmp;
        memcpy(acc + acc_len, buffer, chunk_len);
        acc_len += chunk_len;
        acc[acc_len] = '\0';

        if (tokenize_csv_line(acc,
                              delimiter,
                              out_cells,
                              out_count,
                              &in_quotes) < 0) {
            free(acc);
            return PARSE_ALLOC_FAIL;
        }

        if (!in_quotes) {
            free(acc);
            return PARSE_OK;
        }

        free_cells(*out_cells, *out_count);
        *out_cells = NULL;
        *out_count = 0;

    } while (fgets(buffer, sizeof(buffer), fp));

    free(acc);
    return PARSE_UNTERMINATED_QUOTE;
}
