#include"optimizer.h"

int cmp_notcob_asc(const t_channel *a, const t_channel *b)
{
    if(a->not_cob < b->not_cob)
        return(-1);
    if(a->not_cob > b->not_cob)
        return(1);
    return(0);
}

int cmp_n_asc(const t_channel *a, const t_channel *b)
{
    return (a->n - b->n);
}

t_channel* sorted_merge(t_channel* a, t_channel* b, t_cmp cmp)
{

    if (!a) return b;
    if (!b) return a;

    if(cmp(a,b) <= 0)
    {
        a->next = sorted_merge(a->next, b, cmp);
        return a;
    }
    else
    {
        b->next = sorted_merge(a, b->next, cmp);
        return b;
    }
}

void split_list(t_channel* source, t_channel** front, t_channel** back)
{
    t_channel* slow = source;
    t_channel* fast = source->next;

    // Fast advances 2 nodes, slow 1 node
    while (fast)
    {
        fast = fast->next;
        if (fast)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void merge_sort(t_channel** head_ref, t_cmp cmp)
{
    t_channel* head = *head_ref;
    t_channel *a, *b;

    if (!head || !head->next)
        return;

    split_list(head, &a, &b);

    merge_sort(&a, cmp);
    merge_sort(&b,cmp);

    *head_ref = sorted_merge(a, b, cmp);
}