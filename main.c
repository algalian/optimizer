#include "optimizer.h"


/*
** Expected usage (for now):
**
**   ./optimizer <input_file>
**
** Column names and other parameters will be configurable later.
*/

int main(int argc, char **argv)
{
    const char *input_path;
    t_channel  *channels = NULL;
    t_globals   globals = {0};
    char *end;
    long double val;

    /* --------------------------------------------------
    ** ARGUMENT VALIDATION
    ** -------------------------------------------------- */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    input_path = argv[1];

    /* --------------------------------------------------
    ** DEFAULT FIELD MAPPING
    **
    ** Order MUST match expectations in build_colmap()
    ** -------------------------------------------------- */
    char *fields[] = 
    {        
        "A",             
        "B",             
        "C",             
        "CPM Compra",
        "Channel",             
        "Corr Dupl",   
        "Universo 000",      
        "0.00217",//support ratios  
        "Simulador",       
        NULL
    };

    /* --------------------------------------------------
    ** LOAD DATA
    ** -------------------------------------------------- */
    if (load_channels_from_file(input_path, fields, &channels, &globals) != 0) 
    {
        fprintf(stderr, "Error: failed to load channels from '%s'\n", input_path);
        free_channels(channels);
        return EXIT_FAILURE;
    }
    val = strtold(fields[7], &end);
    globals.acc = val;
    printf("data loaded into channels list. Displaying:\n");
    display_channels(channels, &globals);
    logic_engine(&channels, &globals);
    free_channels(channels);
    return EXIT_SUCCESS;
}