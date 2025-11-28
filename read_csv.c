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

static void display_channels(t_channel *t) //DEBUGGING ONLY
{
    t_channel *aux;

    aux = t;
    while(aux)
    {
        if(aux->name)
            printf("%s\n", aux->name);
        if(aux->a)
            printf("a:%i\n", aux->a);
        /*if(aux->b)
            printf("b:%s\n", aux->b);
        if(aux->c)
            printf("c:%s\n", aux->c);
        if(aux->cpm)
            printf("cpm:%s\n", aux->cpm);
        if(aux->universe)
            printf("universe:%s\n", aux->universe);*/
        aux = aux->next;
        
    }
}

static void populate_name(t_channel *aux, char *line, int field_len, int i)
{
    int j;

    aux->name = malloc(sizeof(char) * (field_len+1));
    if(!aux->name)
    {
        perror("malloc error populating channel struct (name)\n");
        exit(1);
    }
        
    j = 0;
    while(j < field_len)
    {
        aux->name[j] = line[i];
        i++;
        j++;
    }
    aux->name[j] = '\0';
}

static void populate_a(t_channel *aux, char *line, int field_len, int i)
{
    int j;
    char *s;
    char *endptr;

    s = malloc(sizeof(char) * (field_len + 1));
    if(!s)
    {
        perror("malloc error populating channel struct (A)\n");
        exit(1);
    }        
    j = 0;
    while(j < field_len)
    {
        s[j] = line[i];
        i++;
        j++;
    }
    s[j] = '\0';
    aux->a = strtod(s, &endptr);
    if(*endptr != '\0' || endptr == s)
    {
        printf("parameter A in row %i is not recognized as a numerical value", i);
        exit(1);
    }
}




static bool retrieve_field(int pos, t_channel *t, int fd, char *field)
{
    int i;
    int k;
    int iter;
    char *line;
    int commas;
    int field_len;
    t_channel *aux;

    aux = t;
    line = get_next_line(fd);
    printf("%s\n",line);
    //display_channels(t);
    iter = 0;
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
            printf("the loop was broken at %i i'm out\n", iter);
            break;        
        }
        field_len = 0;
        k = i;
        while(line[k] != ',' && line[k])
        {   
            field_len++;
            k++;
        }
        //printf("len : %i, char %c\n", field_len, line[i]);
        if(strcmp(field, "name") == 0)
        {    
            populate_name(aux, line, field_len, i);
        }
        if(strcmp(field, "A") == 0)
        {
            printf("it does know what it's gotta do\n");
            populate_a(aux, line, field_len, i);
        }
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
        iter++;
    }
    //display_channels(t);
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
    int pos;
    bool names;
    bool a;
    /*bool b;
    bool c;
    bool cpm;
    bool universe;*/
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
    a = false;
    while(1 && i < 20)
    {
		line = get_next_line(fd);
        if (line == NULL)
		{
			break;
		}
        //printf("%s\n", line);
		needle = strstr(line,",Channel,");
        if(!names && needle)
        {    
            //printf("Haystack:%s\nNeedle:%s\n needdle - line: %li\n", line, needle, needle- line);
            pos = locate_pos(needle, line);
            /*printf("pos: %i\nline: %s\n", i, line);
            exit(0);*/
            names = retrieve_field(pos, t, fd, "name");
            /*display_channels(t);
            exit(0);*/
            close(fd);
            fd = open(filename, O_RDONLY);
            if(fd < 0)
            {
                fprintf(stderr, "Error: cannot open file '%s'\n", filename);
                exit(1);
            }
            line = get_next_line(fd); //careful. This means that the headers cannot be in the first row.
            //
        }
        needle = strstr(line, ",A,");
        if(!a && needle)
        {
            pos = locate_pos(needle, line);
            a = retrieve_field(pos, t, fd, "A");
            /*display_channels(t);
            exit(0);*/            
        }
        if(line)
            free(line);
        i++;
	}
    //printf("lines %i\n", i);
    close(fd);
    return(t);
}