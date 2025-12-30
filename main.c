#include "optimizer.h"

int main(int argc, char **argv)
{
    t_parser parser;
    t_channel t;
    t_globals g;
    char **cols;
    int count;
    int i;
    char **s;

    count = 0;
    cols = NULL;
    if(argc != 10) 
    { 
        s = malloc(sizeof(char *) * 9); 
        s[0] = argv[1]; 
        s[1] = "A"; 
        s[2] = "B"; 
        s[3] = "C"; 
        s[4] = "CPM"; 
        s[5] = "Corr. Dupl";
        s[6] = "Channel";
        s[7] = "Universo"
        s[8] = "0.001"; // or X/Y. Add to readme.
    } 
    else 
        s = argv;
    use_csv_parser(&parser);
    if (parser.open(&parser, argv[1]) != 0)
        return dprintf(2, "Failed to open file\n"), 1;
    parser.read_header(&parser, &cols, &count);
    /*printf("HEADER COLUMNS (%d):\n", count);
    i = 0;
    while(i < count)
    {
        printf("[%s]\n", cols[i]);
        i++:
    }*/
    if (load_channels_from_file(argv[1], s, &t, &g) != 0)
        return (fprintf(stderr, "File loading failed\n"), 1);
    free_cells(cols, count);
    parser.close(&parser);
}