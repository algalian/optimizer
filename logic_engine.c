#include"optimizer.h"

static long double restricted_pow(long double x, long double y)
{
	if(x == 0)
		return(0);
	return(pow(x,y));
}



static void compute_cob(t_channel *t, t_globals *g, int inv)
{
	long double cob;
	long double x1;
	long double x2;
	long double x3;
	long double x4;
	long double x;

	x1 = g->universe/100;

	x2 = t->cpm*x1;

	x3 = inv/x2;
	x4 = x3 * 0.001;

	x = inv/(t->cpm*(g->universe/100));
    x = x*0.001;

	x1 = restricted_pow(x4, (t->c)); //handle the zero case
	x2 = x1 * t->b;
	x3 = x2 + 1;
	cob = t->a / x3;
	cob = t->a/(1 + (t->b*restricted_pow(x, (t->c)))); // TO DO: use a more performative power func
	t->cob = cob;
	t->not_cob = 1 - cob;
	t->inv = inv;
}

static double second_tier_aggregated_cob(t_channel *t, t_globals *g)
{
   int i;
   long double ag_cob;
   t_channel *snd;

	
	snd = t;
	snd = snd->next;
	//display_channels(snd, NULL);
	i = 0;   
	ag_cob = 1;
	while(snd)
	{
		ag_cob = ag_cob * pow((snd->not_cob), pow(g->beta,i));
		i++;
		snd = snd->next;
	}

	return(1- ag_cob);
}

static double aggregated_cob(t_channel *t, t_globals *g, long double snd_ag_cob)
{
	if(t->not_cob < snd_ag_cob)
		return(1 - (t->not_cob*pow(snd_ag_cob, g->alpha)));
	else 
		return(1 - (snd_ag_cob*pow(t->not_cob, g->alpha)));
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
		n->not_cob = src->not_cob;
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
		dst->not_cob = src->not_cob;
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
	long double max;
	long double snd_ag_cob;
	long double ag_cob;
	t_channel *tmp;
	int n_channels;
	int i;
	int j;
	int sum;
	time_t last_report = 0;
	unsigned long long checked;
	bool max_found;

	n_channels = count_channels(*t);
	total = 1000000; // this might be flexible, dynamic or user-defined?
	acc = 1000;
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
		//display_channels(*t, NULL);
		merge_sort(t, cmp_notcob_asc);
		//display_channels(*t, NULL);
		snd_ag_cob = second_tier_aggregated_cob(*t, g);
		//printf("snd tier ag cob: %f\n",snd_ag_cob);
		ag_cob = aggregated_cob(*t,g,snd_ag_cob);
		merge_sort(t, cmp_n_asc);
		//printf("ag cob: %f\n", ag_cob);
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
			//display_channels(opt, NULL);
			//printf("with cob %Lf\n ", ag_cob);
			i = 0;
			while(i < n_channels)
			{
				printf("%i,", inv[i]);
				i++;
			}
			printf("\n");
			max_found = true;
			//printf("new max is %f\n", max);
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
            printf("%llu combinations checked. Current: %i, %i, %i, %i. Cob = %20f\n", checked, inv[0], inv[1], inv[2], inv[3], ag_cob);
			fflush(stdout);
            last_report = now;
			max_found = false;
        }
		checked++;
	}
    //merge_sort(t, cmp_n_asc);
	printf("found it!!!  coberture = %20Lf\n", max);
	display_channels(opt, g);
	free(inv);
}