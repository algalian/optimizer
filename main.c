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
    char *fields[] = {
        NULL,           /* [0] unused (argv compatibility removed) */
        "A",             /* [1] */
        "B",             /* [2] */
        "C",             /* [3] */
        "CPM",           /* [4] */
        "Corr. Dupl",    /* [5] (currently unused) */
        "Channel",       /* [6] */
        "Universo",      /* [7] (currently unused) */
        "0.001",         /* [8] alpha or ratio placeholder */
        NULL
    };

    /* --------------------------------------------------
    ** LOAD DATA
    ** -------------------------------------------------- */
    if (load_channels_from_file(input_path, fields, &channels, &globals) != 0) {
        fprintf(stderr, "Error: failed to load channels from '%s'\n", input_path);
        free_channels(channels);
        return EXIT_FAILURE;
    }

    free_channels(channels);
    return EXIT_SUCCESS;
}