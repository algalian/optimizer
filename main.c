#include "optimizer.h"




int main(int argc, char **argv)
{
    t_channel *t;
    t_channel *tmp;
    t_globals g;


    t = NULL;
    read_csv(argv[1], &t, &g);
    //printf("File succesfully read and loaded. Displaying parameters...\n");
    //display_channels(t, &g);
    //printf("proceeding to calculate. Please wait...\n");
    logic_engine(&t, &g);
    return(0);
}