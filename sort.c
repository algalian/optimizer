#include"optimizer.h"

t_channel* sorted_merge(t_channel* a, t_channel* b)
{
    t_channel* result = NULL;

    if (!a) return b;
    if (!b) return a;

    if (a->cob >= b->cob)
    {
        result = a;
        result->next = sorted_merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = sorted_merge(a, b->next);
    }

    return result;
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

void merge_sort(t_channel** head_ref)
{
    t_channel* head = *head_ref;
    t_channel *a, *b;

    if (!head || !head->next)
        return;

    split_list(head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);

    *head_ref = sorted_merge(a, b);
}