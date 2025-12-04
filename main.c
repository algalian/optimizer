#include "optimizer.h"


unsigned int n_channels;


int main(int argc, char **argv)
{
    t_channel *t;


    if(argc <= 7)
    {
        n_channels = 5;
    }
    if(argc > 7)
    {
        n_channels = argc;
    }
    t = NULL;
    read_csv(argv[1], &t);
    logic_engine();
    return(0);
}