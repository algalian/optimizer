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


void display_channels(t_channel *t) //DEBUGGING ONLY
{
    t_channel *aux;
    int i;

    aux = t;
    
    i = 0;
    while(aux)
    {

        printf("%s\n", aux->name);
        printf("a:%f\n", aux->a);
        /*if(aux->b)
            printf("b:%s\n", aux->b);
        if(aux->c)
            printf("c:%s\n", aux->c);
        if(aux->cpm)
            printf("cpm:%s\n", aux->cpm);
        if(aux->universe)
            printf("universe:%s\n", aux->universe);*/
        aux = aux->next;
        //printf("it passes %i succesfully\n", i);
        i++;
        
    }
}

