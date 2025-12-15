#include"optimizer.h"




void compute_cob(t_channel *t, t_globals *g, int inv)
{
    double cob;
    double x;

    x = inv/(t->cpm*(g->universe/100));
    cob = t->a/(1 + pow(x, -(t->c))); // TO DO: use a more performative power func
    t->cob = 1 - cob;
}

double second_tier_aggregated_cob(t_channel *t, t_globals *g)
{
   int i;
   double ag_cob;
   t_channel *snd;

    
    merge_sort(&t);
    snd = t;
    snd = snd->next;
    i = 0;   
    while(snd)
    {
        ag_cob = pow(snd->cob, pow(g->beta,i));
        i++;
        snd = snd->next;
    }
    return(1-ag_cob);
}

double aggregated_cob(t_channel *t, t_globals *g, double snd_ag_cob, int inv)
{
    compute_cob(t, g, inv); // first tier. The list is assumed to be sorted.
    if(t->cob > snd_ag_cob)
        return(1- (t->cob*pow(snd_ag_cob,g->alpha)));
    else 
        return(1 - (snd_ag_cob*pow(t->cob, g->alpha)));
}

void logic_engine(t_channel **t, t_globals *g)
{
    int inv;
    int total;
    double snd_ag_cob;
    double fst_ag_cob;
    t_channel *tmp;


    return;
}