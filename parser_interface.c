#include "optimizer.h"



void free_cells(char **cells, int count)
{
    if (!cells) return;
    for (int i = 0; i < count; i++)
        free(cells[i]);
    free(cells);
}

t_sniff_status sniff_delimiter(const char *path, char *out_delim)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return SNIFF_ERR_OPEN;

    char buf[4096];
    if (!fgets(buf, sizeof(buf), fp)) {
        fclose(fp);
        return SNIFF_ERR_EMPTY;
    }
    fclose(fp);

    int tabs = 0, commas = 0, semis = 0;
    for (int i = 0; buf[i]; i++) {
        if (buf[i] == '\t') tabs++;
        else if (buf[i] == ',') commas++;
        else if (buf[i] == ';') semis++;
    }

    if (tabs > commas && tabs > semis) {
        *out_delim = '\t';
        return SNIFF_OK;
    }
    if (commas > tabs && commas > semis) {
        *out_delim = ',';
        return SNIFF_OK;
    }
    if (semis > tabs && semis > commas) {
        *out_delim = ';';
        return SNIFF_OK;
    }

    return SNIFF_ERR_AMBIGUOUS;
}

int make_parser_for_file(const char *filename, t_parser *out)
{
    const char *dot = strrchr(filename, '.');

    if (dot) {
        if (!strcmp(dot, ".tsv")) {
            use_tsv_parser(out);
            return 0;
        }
        if (!strcmp(dot, ".csv")) {
            use_csv_parser(out);
            return 0;
        }
        if (!strcmp(dot, ".xlsx") || !strcmp(dot, ".xlsm")) 
        {
           // printf("using xlsx parser\n");
            use_xlsx_parser(out);
            return 0;
        }
    }

    char d;
    if (sniff_delimiter(filename, &d) != SNIFF_OK)
        return -1;

    if (d == '\t')
        use_tsv_parser(out);
    else
        use_csv_parser(out);

    out->delimiter = d;
    return 0;
}

