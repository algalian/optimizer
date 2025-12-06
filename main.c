#include "optimizer.h"




int main(int argc, char **argv)
{
    t_channel *t;
    t_channel *tmp;
    t_globals g;


    t = NULL;
    read_csv(argv[1], &t, &g);
    printf("File succesfully read and loaded. Displaying parameters...\n");
    tmp = t;
    while (tmp) 
    {
        printf("Channel: %s, a = %f, b = %f, c = %f, cpm = %f, n = %i\n", tmp->name, tmp->a, tmp->b, tmp->c, tmp->cpm, tmp->n);
        tmp = tmp->next;
    }
    printf("with globals alpha=%f, beta=%f universe=%f\n",g.alpha, g.beta, g.universe);
    //printf("proceeding to calculate. Please wait...\n");
    logic_engine(&t, &g);
    return(0);
}