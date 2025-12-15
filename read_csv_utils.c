#include"optimizer.h"


bool is_line_empty(char *s)
{
	int i;

	i = 0;
	while(s[i] && isprint(s[i]))
		i++;
	if(i == 0)
		return(true);
	return(false);
	
}


void display_channels(const t_channel *head, const t_globals *g)
{
    printf("Globals:\n");
    printf("  alpha    : %.3f\n", g->alpha);
    printf("  beta     : %.3f\n", g->beta);
    printf("  universe : %.3f\n", g->universe);

    printf("\nChannels:\n");

    const t_channel *tmp = head;
    int index = 0;

    while (tmp) {
        printf(" Channel %s, a = %f, b = %f, c = %f, cpm = %f, Nº = %i, coberture = %f\n",
               tmp->name,
               tmp->a,
               tmp->b,
               tmp->c,
               tmp->cpm,
               tmp->n,
               tmp->cob);

        tmp = tmp->next;
        index++;
    }

    if (index == 0)
        printf("  (no channels)\n");
}
