#include"optimizer.h"

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