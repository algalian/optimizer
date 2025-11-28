#include"optimizer.h"



/*static bool is_line_empty(char *s)
{
    int i;

    i = 0;
    while(s[i] && isprint(s[i]))
        i++;
    if(i == 0)
        return(true);
    return(false);
    
}*/

static void display_channels(t_channel *t)
{
    t_channel *aux;

    aux = t;
    while(aux)
    {
        if(aux->name)
            printf("%s\n", aux->name);
        /*if(aux->a)
            printf("a: %i\n", aux->a);
        if(aux->b)
            printf("b:%i\n", aux->b);
        if(aux->c)
            printf("c:%i\n", aux->c);
        if(aux->cpm)
            printf("cpm:%i\n", aux->cpm);
        if(aux->universe)
            printf("universe:%i\n", aux->universe);*/
        aux = aux->next;
        
    }
}


static bool retrieve_names(int pos, t_channel *t, int fd)
{
    int i;
    int j;
    int k;
    char *line;
    int commas;
    int name_len;
    t_channel *aux;

    aux = t;
    line = get_next_line(fd);
    while(1)
    {
        i = 0;
        commas = 0;
        while(commas <= pos)
        {
            if(line[i] == ',')
            {
                commas++;
            }
            i++;
            //printf("%i: %c\n", i, line[i]);
        }
        if(line[i] == ',')
        {
            free(line);
            //printf("the loop was broken, i'm out\n");
            break;        
        }
        name_len = 0;
        k = i;
        while(line[k] != ',' && line[k])
        {   
            name_len++;
            k++;
        }
        //printf("len : %i, char %c\n", name_len, line[i]);
        aux->name = malloc(sizeof(char) * (name_len+1));
        if(!aux->name)
        {
            perror("malloc error populating names in channel struct\n");
            exit(1);
        }
        j = 0;
        while(j < name_len)
        {
            aux->name[j] = line[i];
            i++;
            j++;
        }
        aux->name[j] = '\0';
        aux->next = malloc(sizeof(t_channel));
        if(!aux->next)
        {
            perror("malloc error generating channel struct");
            exit(1);
        }
        /*printf("%s\n", aux->name);
        exit(0);*/
        aux = aux->next;
        aux->next = NULL;
        free(line);
        line = get_next_line(fd);
    }
    display_channels(t);
    exit(0);
    return(true);
}

static int locate_pos(char *needle, char *line)
{
    int commas;
    int len;
    int i;

    len = 0;
    while(needle[len + 1] != ',')
    {   
        len++;
    }
    needle[len+1]= '\0';
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

t_channel *read_csv(char *filename) 
{
    int fd;
    int i;
    char *line;
    char *needle;
    bool names;
    int pos;
    t_channel *t;

    fd = open(filename, O_RDONLY);
    if(fd < 0)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        exit(1);
    }
    i = 0;
    t = malloc(sizeof(t_channel));
    names = false;
    while(1)
    {
		line = get_next_line(fd);
        if (line == NULL)
		{
			break;
		}
        //printf("%s\n", line);
		needle = strstr(line,",Channel,");
        if(needle && !names)
        {    
            //printf("Haystack:%s\nNeedle:%s\n needdle - line: %li\n", line, needle, needle- line);
            pos = locate_pos(needle, line);
            /*printf("pos: %i\nline: %s\n", i, line);
            exit(0);*/
            names = retrieve_names(pos, t, fd);
        }
        free(line);
        i++;
	}
    //printf("lines %i\n", i);
    close(fd);
    return(t);
}