#include"optimizer.h"

void display_channels(const t_channel *head, const t_globals *g)
{
	if(g)
	{
		int acc;
		acc = g->budget * g->acc;
		while((int)g->budget % acc != 0)
			acc--;
		printf("Globals:\n");
		printf("  alpha    : %Lf\n", g->alpha);
		printf("  beta     : %Lf\n", g->beta);
		printf("  universe : %Lf\n", g->universe);
		printf("  budget   : %LF\n", g->budget);
		printf("  granularity : %i\n", acc);
	}
	printf("\nChannels:\n");
	const t_channel *tmp = head;
	int index = 0;
	while (tmp) 
	{
		printf(" Channel %s, a = %Lf, b = %Lf, c = %Lf, cpm = %Lf, Nº = %i, coberture = %Lf, inv = %i, not_cob %Lf\n",
		tmp->name,
		tmp->a,
		tmp->b,
		tmp->c,
		tmp->cpm,
		tmp->n,
		tmp->cob, tmp->inv, tmp->not_cob);
		tmp = tmp->next;
		index++;
	}
	if (index == 0)
		printf("  (no channels)\n");
}

int count_channels(t_channel *t)
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


void free_channels(t_channel *head)
{
    t_channel *tmp;

    while (head)
    {
        tmp = head->next;   
        free(head);         
        head = tmp;         
    }
}

t_channel *new_node(void)
{
    t_channel *n = calloc(1, sizeof(t_channel));
    return n;
}

void append_node(t_channel **list, t_channel *node)
{
    if (!*list) {
        *list = node;
        return;
    }
    t_channel *tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = node;
}