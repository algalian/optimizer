#include"optimizer.h"



static void compute_cob(t_channel *t, t_globals *g, int inv)
{
	double cob;
	double x;

	x = inv/(t->cpm*(g->universe/100));
    x = x*0.001;
	cob = t->a/(1 + (t->b*pow(x, (t->c)))); // TO DO: use a more performative power func
	t->cob = cob;
	t->inv = inv;
}

static double second_tier_aggregated_cob(t_channel *t, t_globals *g)
{
   int i;
   double ag_cob;
   t_channel *snd;

	
	snd = t;
	snd = snd->next;
	//display_channels(snd, g);
	i = 0;   
	ag_cob = 1;
	while(snd)
	{
		ag_cob *= pow((1-snd->cob), pow(g->beta,i));
		i++;
		snd = snd->next;
	}
	return(ag_cob);
}

static double aggregated_cob(t_channel *t, t_globals *g, double snd_ag_cob)
{
	if(t->cob > snd_ag_cob)
		return(1- (t->cob*pow(snd_ag_cob,g->alpha)));
	else 
		return(1 - (snd_ag_cob*pow(t->cob, g->alpha)));
}

static int count_channels(t_channel *t)
{
	t_channel *tmp;
	int i;
	
	i = 0;
	tmp = t;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return(i);
}
t_channel *copy_list(const t_channel *src) 
{
    t_channel *head;
    t_channel *tail;
    t_channel *n;

	head = NULL;
	tail = NULL;
    while (src != NULL) 
	{
    	n = malloc(sizeof(t_channel));
        if (!n) 
		{
            free_channels(head);
			perror("malloc error in logic engine\n");
			exit(1);
		}
        n->a = src->a;
		n->b = src->b;
		n->c = src->c;
		n->cpm = src->cpm;
		n->cob = src->cob;
		n->inv = src->inv;
		n->n = src->n;
		n->name = src->name;
        n->next = NULL;
        if (!head) 
		{
            head = n;
            tail = n;
        } 
		else 
		{
            tail->next = n;
            tail = n;
        }

        src = src->next;
    }

    return(head);
}

void update_list(t_channel *dst, const t_channel*src) 
{
    while (dst != NULL && src != NULL) 
	{
        dst->a = src->a;
		dst->b = src->b;
		dst->c = src->c;
		dst->cpm = src->cpm;
		dst->cob = src->cob;
		dst->inv = src->inv;
		dst->n = src->n;
		dst->name = src->name;
        dst = dst->next;
        src = src->next;
    }
}

void logic_engine(t_channel **t, t_globals *g)
{
	int *inv;
	t_channel *opt;
	int total;
	int acc;
	double max;
	double snd_ag_cob;
	double ag_cob;
	t_channel *tmp;
	int n_channels;
	int i;
	int j;
	int sum;
	time_t last_report = 0;
	unsigned long long checked;


	n_channels = count_channels(*t);
	total = 1000000; // this might be flexible, dynamic or user-defined?
	acc = 10000;
	inv = malloc(sizeof(int) * n_channels);
	opt = NULL;
	if(!inv)
	{
		perror("malloc error in logic engine\n");
		exit(1);
	}
	i = 1;
	inv[0] = total;
	while(i < n_channels)
	{
		inv[i] = 0;
		i++;
	}
	/*i = 0;
	while(inv[i])
	{
		printf("%i\n", inv[i]);
		i++;
	}*/

	i = 0;
	max = 0;
	checked = 0;
	while(1) // what is the exit condition? 
	{	
		i = 0;
		tmp = *t;
		while(tmp)
		{
			compute_cob(tmp,g,inv[i]);
			//printf("%f\n",tmp->cob);
			tmp = tmp->next;
			i++;
		}
		//display_channels(*t, g);
		merge_sort(t, cmp_cob_dsc);
		//display_channels(*t, NULL);
		snd_ag_cob = second_tier_aggregated_cob(*t, g);
		//printf("snd tier ag cob: %f\n",snd_ag_cob);
		ag_cob = aggregated_cob(*t,g,snd_ag_cob);
		merge_sort(t, cmp_n_asc);
		//display_channels(*t,NULL);
        if(ag_cob >= max)
		{	
			max = ag_cob;
			if(opt == NULL)
			{
				opt = copy_list(*t);
				if(!opt)
				{
					perror("malloc error in logic engine\n");
					exit(1);
				}

			}
			else
			{
				update_list(opt, *t);
			}

		}
		//display_channels(*t, g);
		//printf("%f\n", ag_cob);
		i = n_channels - 2;
		while(i >= 0 && inv[i] == 0)
		{
			i--;
		}
		if(i < 0)
			break;
		inv[i] = inv[i] - acc;
		sum = 0;
		j = i + 1;
		while(j < n_channels)
		{
			sum += inv[j];
			inv[j] = 0;
			j++;
		}
		if (i + 1 < n_channels)
			inv[i+1] = sum + acc;
		time_t now = time(NULL);
        if (now - last_report >= 1) 
		{
            printf("%llu combinations checked. Current: %i, %i, %i, %i\n", checked, inv[0], inv[1], inv[2], inv[3]);
			fflush(stdout);
            last_report = now;
        }
		checked++;
	}
    //merge_sort(t, cmp_n_asc);
	printf("found it!!!  coberture = %f\n", max);
	display_channels(opt, g);
	free(inv);
}