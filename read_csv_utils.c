#include"optimizer.h"


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

int locate_pos(char *needle, char *line)
{
    int commas;
    int len;
    int i;

    len = 0;
    while(needle[len + 1] != ',')
    {   
        len++;
    }
    needle[len + 1]= '\0';
    commas = 0;
    len = strlen(needle);
    i = 0;
    while(strncmp(needle, line, len) != 0)
    {
        if(line[0] == ',')
            commas++;
        line++;
        i++;
    }
    return(commas);
}
