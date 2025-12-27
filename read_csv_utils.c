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
	if(g)
	{
		printf("Globals:\n");
		printf("  alpha    : %Lf\n", g->alpha);
		printf("  beta     : %Lf\n", g->beta);
		printf("  universe : %Lf\n", g->universe);
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
