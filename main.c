#include "optimizer.h"


unsigned int n_channels;


int main(int argc, char **argv)
{
    t_channel *t;
    t_channel *tmp;

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
    tmp = t;
    while (tmp) 
    {
        printf("Channel: %s, a = %f, b = %f, c = %f, cpm = %f, n = %i\n", tmp->name, tmp->a, tmp->b, tmp->c, tmp->cpm, tmp->n);
        tmp = tmp->next;
    }
    logic_engine();
    return(0);
}