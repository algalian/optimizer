#include "optimizer.h"

int main(int argc, char **argv)
{
    t_parser parser;
    char **cols;
    int count;
    int i;

    count = 0;
    cols = NULL;
    use_csv_parser(&parser);
    if (parser.open(&parser, argv[1]) != 0)
        return dprintf(2, "Failed to open file\n"), 1;
    parser.read_header(&parser, &cols, &count);
    printf("HEADER COLUMNS (%d):\n", count);
    i = 0;
    while(i < count)
    {
        printf("[%s]\n", cols[i]);
        i++:
    }
    free_cells(cols, count);
    parser.close(&parser);
}