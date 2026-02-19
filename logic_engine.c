#include"optimizer.h"

static long double restricted_pow(long double x, long double y)
{
	return(pow(x,y));
}

void free_list(t_channel *head)
{
    t_channel *tmp;

    while (head)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

static bool compute_cob(t_channel *t, t_globals *g, int inv, bool contrast)
{
	long double cob;
	long double x1;
	long double x2;
	long double x3;
	long double x4;
	long double x5;
	long double x6;
	long double x7;

	t->inv = inv;
	if(t->inv == 0)
	{
		t->cob = 0;
		t->not_cob = 1;
		return(contrast);
	}
	x1 = g->universe/100;
	x2 = t->cpm*x1;

	x3 = t->inv/x2;
	x4 = x3 * 0.001;

	//x = inv/(t->cpm*(g->universe/100));
	//x = x*0.001;

	x5 = restricted_pow(x4, (t->c)); //handle the zero case
	x6 = x5 * t->b;
	x7 = x6 + 1;
	cob = t->a / x7;
	//cob = t->a/(1 + (t->b*restricted_pow(x, (t->c)))); // TO DO: use a more performative power func
	t->cob = cob;
	t->not_cob = 1 - cob;
	if(contrast == true)
	{	
		printf("inv: %i\nx1:%Lf\nx2:%Lf\nx3:%Lf\nx4:%Lf\nx5:%LF\nx6:%Lf\nx7:%Lf\ncob:%Lf\nnot_cob:%LF\n",inv,x1,x2,x3,x4,x5,x6,x7,t->cob,t->not_cob);
		contrast = false;
	}
	return(contrast);
}

static double second_tier_aggregated_cob(t_channel *t, t_globals *g)
{
	int i;
	long double prod;
	t_channel *snd;
	long double exp;
	long double value;
	
	if(t != NULL)
	{
		snd = t;
		snd = snd->next;
	}
	else
		snd = NULL;
	//display_channels(snd, g);
	//exit(0);
	i = 0;   
	prod = 1.0;
	while(snd)
	{
		exp = restricted_pow(g->alpha, i);
		value = pow(snd->not_cob, exp);
		prod *= value;
		//printf("%Lf %Lf %Lf\n", exp, value, snd_ag_cob);
		i++;
		snd = snd->next;
	}
	return(1 - prod);
}

static double aggregated_cob(t_channel *t, t_globals *g, long double snd_ag_cob)
{
	t_channel *fst;
	long double x1;
	long double x2;
	//long double x3;
	long double not_snd_ag_cob;


	not_snd_ag_cob = 1 - snd_ag_cob;
	fst = t; 
	if(fst->not_cob <  not_snd_ag_cob)
	{
		//printf("case 1 true\n");
		x1 = restricted_pow(not_snd_ag_cob, g->beta);
		x2 = fst->not_cob*x1;
	}
	else
	{
		//printf("case 2 true\n");
		x1 = restricted_pow(fst->not_cob, g->beta);
		x2 = not_snd_ag_cob * x1;
	}
	return(1 - x2);
}



t_channel *logic_engine(t_channel **t, t_globals *g)
{
	int *inv;
	t_channel *opt;
	long double snd_ag_cob;
	long double ag_cob;
	t_channel *tmp;
	int n_channels;
	int i;
	int j;
	int k;
	int sum;
	time_t last_report = 0;
	bool max_found;
	bool contrast;
	int round;
	int sum_mins;
	int total; 
	int acc;


	//display_channels(*t, g);
	n_channels = count_channels(*t); 
	total = g->budget; 
	acc = total * g->acc;
	if(acc != 0)
	{
		while(total % acc != 0)
			acc--;
		g->grn = acc;
	}
	else
		g->grn = 1;
	//display_channels(*t, g);
	//exit(0);
	inv = malloc(sizeof(int) * n_channels);
	if(!inv)
	{
		perror("malloc error in logic engine\n");
		exit(1);
	}
	opt = NULL;
	tmp = *t;
	//display_channels(*t, g);
	while(tmp)
	{
		while((int) tmp->min % (int) g->grn != 0)
		{
			tmp->min++;
		}
		tmp = tmp->next;
	}
	//display_channels(*t, g);
	//exit(0);
	tmp = *t;
	sum_mins = 0;
	while(tmp)
	{
		sum_mins += tmp->min;
		tmp = tmp->next;
	}
	if(sum_mins >= g->budget)
	{
		printf("incompatible solution\n");
		free(inv);
		free_channels(*t);
		exit(0);
	}
	i = 1;
	tmp = *t;
	inv[0] = g->budget - sum_mins + tmp->min;
	tmp = tmp->next;
	while(i < n_channels)
	{
		inv[i] = tmp->min;
		tmp = tmp->next;
		i++;
	}
	i = 0;
	g->max = 0;
	g->iterations = 0;
	contrast = false;
    //while(g->iterations < 1000)
	while(1) // what is the exit condition? 
	{	
		k = 0;
		tmp = *t;
		while(tmp)
		{
			contrast = compute_cob(tmp,g,inv[k], contrast);
			tmp = tmp->next;
			k++;
		}
		t_channel *sorted = copy_list(*t);
		merge_sort(&sorted, cmp_notcob_asc);
		snd_ag_cob = second_tier_aggregated_cob(sorted, g);
		ag_cob = aggregated_cob(sorted,g,snd_ag_cob);
		free_list(sorted);
		if(ag_cob >= g->max)
		{	
			g->max = ag_cob;
			g->max_snd = snd_ag_cob;
			free_list(opt);
			opt = copy_list(*t);
			if(!opt)
			{
				perror("malloc error in logic engine\n");
				exit(1);
			}
			max_found = true;

		}
		time_t now = time(NULL);
		//if(1)
		//if(max_found == true)
		if (now - last_report >= 1) 
		{
			printf("%llu combinations checked. Current vector of inv: %i, %i, %i, %i. Cob = %Lf\n", g->iterations, inv[0], inv[1], inv[2], inv[3], ag_cob);
			///printf("%lli) %i, %i, %i, %i\n", g->iterations, inv[0], inv[1], inv[2], inv[3]);
			fflush(stdout);
			last_report = now;
			max_found = false;
		}


		/* Find rightmost i < N-1 with a[i] > 0 */
		i = n_channels - 2;
		tmp = *t;
		while(tmp->n < n_channels - 1)
		{
			tmp = tmp->next;			
		}
		int min = tmp->min;
		while(i >= 0 && (inv[i] == 0 || inv[i] <= min))
		{
			i--;
			int c = 0;
			tmp = *t;
			while(c < i)
			{
				tmp = tmp->next;
				c++;
			}
			min = tmp->min;
		}
		//printf("rightmost: %i with min: %i\n", i, (int) tmp->min);

		/* If none found, we are done */
		if(i < 0)
			break;

        /* Take one dollar from a[i] */
		tmp = *t;
		while(tmp->n != i + 1)
		{
			tmp = tmp->next;
		}
		if(inv[i] - g->grn >= tmp->min)
		{
			//printf("removing %i from channel %i\n",g->grn, i);
			inv[i] = inv[i] - g->grn;
		}
		else
		{
			//printf("not allowed. Skip\n");
			g->iterations++;
			continue;
		}

		/* Collect all dollars to the right */
		sum = 0;
		j = i + 1;
		//printf("beginning at %i:\n", j);
		tmp = tmp->next;
		while(j < n_channels)
		{
			sum += inv[j];
			sum -= tmp->min;
			inv[j] = tmp->min;
			//printf("collecting %i - %i from channel %i (n = %i)\n", inv[j], (int) tmp->min, j, tmp->n);
			tmp = tmp->next;
			j++;
		}
		//printf("%i dollars collected\n", sum);

		
        /* Give collected dollars + 1 to a[i+1] */
		inv[i+1] += sum + g->grn;
		//printf("channel %i now has %i\n", i+1, inv[i+1]);
		g->iterations++;
	}
	//merge_sort(t, cmp_notcob_asc);
	//display_channels(*t, NULL);
	//exit(0);
	printf("found it!!! displaying solution...\n");
	display_channels(opt, g);
	free(inv);
	return(opt);
}