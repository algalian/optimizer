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

	
	snd = t;
	snd = snd->next;
	display_channels(snd, g);
	i = 0;   
	while(snd)
	{
		ag_cob = pow(snd->cob, pow(g->beta,i));
		i++;
		snd = snd->next;
	}
	return(1 - ag_cob);
}

double aggregated_cob(t_channel *t, t_globals *g, double snd_ag_cob, int inv)
{
	printf("first tier: %f\n", t->cob);
	if(t->cob > snd_ag_cob)
		return(1- (t->cob*pow(snd_ag_cob,g->alpha)));
	else 
		return(1 - (snd_ag_cob*pow(t->cob, g->alpha)));
}

void logic_engine(t_channel **t, t_globals *g)
{
	int inv;
	int acc;
	double max;
	double snd_ag_cob;
	double ag_cob;
	t_channel *tmp;

	tmp = *t;
	inv = 2;
	while(tmp)
	{
		compute_cob(tmp,g,inv);
		//printf("%f\n",tmp->cob);
		tmp = tmp->next;
	}
	//display_channels(*t, g);
	merge_sort(t);
	//display_channels(*t, g);
	snd_ag_cob = second_tier_aggregated_cob(*t, g);
	printf("snd tier ag cob: %f\n",snd_ag_cob);
	ag_cob = aggregated_cob(*t,g,snd_ag_cob, inv);
	//display_channels(*t, g);
	return;
}