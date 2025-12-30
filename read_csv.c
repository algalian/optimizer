#include"optimizer.h"







t_channel *add_channel(t_channel **head)
{
	t_channel *new_node;
	t_channel *tmp;

	new_node = malloc (sizeof(t_channel));
    if (!new_node)
	{
		perror("malloc error creating channel struct\n");
		exit(1);
	}
    new_node->next = NULL;
    if (*head == NULL) 
	{
        *head = new_node;
    } 
	else 
	{
        tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_node;
    }
    return (new_node);
}

int move_to_pos(char *line, int pos)
{
	int i;
	int commas;

	i = 0;
	commas = 0;
	//printf("the line is %s\n the pos is %i", line, pos);
	//exit(0);
	while(commas < pos)
	{
		if(line[i] == ',')
		{
			commas++;
		}
		i++;
		//printf("%i: %c\n", i, line[i]);
	}
	//printf("%s\n", &line[i]);
	//exit(0);
	return(i);
}

void build_channels(t_channel **t, int fd, int pos)
{
	char *line;
	int i;

	while(1)
	{	
		i = 0;
		line = get_next_line(fd);
		i += move_to_pos(line, pos);
		if(line[i] == ',')// this means no more rows are below
		{
			free(line);
			break; 
		}
		add_channel(t);
	}
	close(fd);
}

int field_len(int i, char *line)
{
	int len;
	int j;

	len = 0;
	j = i;
	while(line[j] && line[j] != ',')
	{
		if(line[j] == '"')
		{
			j++;
			len++;
			while(line[j] != '"')
			{
				j++;
				len++;
			}
		}
		j++;
		len++;

	}
	return(len);
}

static void liberate(char **s)
{
	int i;

	i = 0;
	while(s[i])
	{
		if(s[i])
			free(s[i]);
		i++;
	}
	free(s);
}





int locate_pos(char *needle, char *line)
{
	int commas;
	char *s;
	char *dup_line;
	int len;
	int i;

	s = ft_strdup(needle);
	s++;
	dup_line = ft_strdup(line);
	len = strlen(s);
	commas = 0;
	i = 0;
	while(ft_strncmp(s, dup_line, len) != 0)
	{
		if(dup_line[0] == ',')
			commas++;
		dup_line++;
		i++;
	}
	dup_line -= i;
	free(dup_line);
	s--;
	free(s);
	return(commas);
}

static int backtrack(int i, char *filename, int fd)
{
	int new_fd;
	int j;
	char *line;

	close(fd);
	new_fd = open(filename, O_RDONLY);
	if(new_fd < 0)
	{
		fprintf(stderr, "Error: cannot open file '%s'\n", filename);
		exit(1);
	}
	j = 0;
	while(j < i)
	{
		line = get_next_line(new_fd);
		free(line);
		j++;
	}
	return(new_fd);
}

char *retrieve(char *line, int i, int len)
{
	int j;
	int k;
	char *s;

	j = i;
	s = malloc(sizeof(char) * (len + 1));
	k = 0;
	while(line[j] != ',' && line[j])
	{
		s[k] = line[j];
		k++;
		j++;
	}
	s[k] ='\0';
	return(s);
}


static void check_and_assign(double p, char *param, t_channel *t, char *s, char *endptr)
{
	if(strcmp(param,"A") == 0)
	{
		if(*endptr != '\0' || endptr == s)
		{
			printf("Error in param %s. %s is not a number\n", param, s);
			exit(1);
		}
		t->a = p;
	}
	if(strcmp(param, "B") == 0)
	{
		if(*endptr != '\0' || endptr == s)
		{
			printf("Error in param %s. %s is not a number\n", param, s);
			exit(1);
		}
		t->b = p;
	}
	if(strcmp(param, "C") == 0)
	{
		if(*endptr != '\0' || endptr == s)
		{
			printf("Error in param %s. %s is not a number\n", param, s);
			exit(1);
		}
		t->c = p;
	}
	if(strcmp(param, "CPM Compra") == 0)
	{
		if(*endptr != '\0' || endptr == s)
		{
			printf("Error in param %s. %s is not a number\n", param, s);
			exit(1);
		}
		t->cpm = p;
	}
	if(strcmp(param, "Channel") == 0)
	{	
		t->name = ft_strdup(s);
	}
	if(strcmp(param, "Nº") == 0)
		t->n = (int) p;
}

static void populate_channels(t_channel *head, int fd, int pos, char *needle)
{
	int i;
	char *line;
	int len;
	double tmp;
	char *endptr;
	char *s;
	char **header; //check for the header row and add to the corresponding field.
	int k;
	int scan;

	header = ft_split(needle, ',');
	i = 0;
	/*while(header[i])
	{
		printf("%s\n", header[i]);
		i++;
	}
	exit(0);*/
	scan = 0;
	while (head != NULL) 
	{
		i = 0;
		line = get_next_line(fd);
		i = move_to_pos(line, pos);
		k = 0;
		//printf("%s, %i\n", &line[i], i);
		//exit(0);
		while(header[k])	
		{
			len = field_len(i,line);
			if(strcmp(header[k], "A") == 0 
			|| strcmp(header[k], "B") == 0 
			|| strcmp(header[k],"C") == 0 
			|| strcmp(header[k], "CPM Compra") == 0 
			|| strcmp(header[k],"Channel") == 0 
			|| strcmp(header[k], "Nº") == 0)
			{
				s = retrieve(line, i, len);
				//printf("%s\n", s);
				//exit(0);
				tmp = strtod(s, &endptr);
				//printf("%f\n", tmp);
				//exit(0);
				check_and_assign(tmp,header[k], head, s, endptr);
				free(s);
			}
			//printf("%s\n%s\n", header[k],&line[i]);
			i += len;
			i++;
			k++;
		}
		/*printf("%s\n", head->name);
		printf("%f\n", head->a);
		printf("%f\n", head->b);
		printf("%f\n", head->c);
		printf("%f\n", head->cpm);
		//printf("%s\n", head->universe);
		printf("%i\n", head->n);*/
		head = head->next;
		scan++;
    }
	liberate(header);
	//exit(0);
}




static void get_universe(char **args, t_globals *g)
{
	int fd;
	int i;
	char *line;
	char *needle;
	int j;
	int len;
	char *s;
	double tmp;
	char *endptr;

	fd = open(args[0], O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr, "Error: cannot open file '%s'\n", args[0]);
		exit(1);
	}
	i = 0;
	while(1)
	{
		if(line == NULL)
		{
			printf("warning. Universe variables not found\n");
			exit(1);
		}
		line = get_next_line(fd);
		needle = strstr(line, ft_strjoin(",", args[5]));
		if(needle) //TO DO, Refactor and make it resilient to both commas and !commas)
		{
			needle++;
			while(needle[0] != ',')
				needle++;
			len = 0;
			needle += 2;
			j = 0;
			while(needle[j] != '"')
			{
				if(needle[j] != ',')
				{
					len++;
				}
				j++;
			}
			s = malloc(sizeof(char) * (len+1));
			j = 0;
			len = 0;
			while(needle[j] != '"')
			{
				if(needle[j] !=  ',')
				{
					s[len] = needle[j];
					len++;
				}
				j++;
			}
			s[len] = '\0';
			tmp = strtod(s, &endptr);
			if(*endptr != '\0' || endptr == s)
			{
				printf("Error in param %s. %s is not a number\n", args[5],s);
				exit(1);
			}	
			g->universe = tmp;
			free(s);
			free(line);
			return;
		}
		i++;

	}
}

static void load_globals(int pos, int fd, t_globals *g)
{
	int i;
	char *line;
	int len;
	char *s;
	char *endptr;
	float tmp;

	i = 0;
	line = get_next_line(fd);
	//printf("we are here now: %s\n", line);
	//exit(0);
	i = move_to_pos(line, pos);
	len = field_len(i, line);
	s = retrieve(line, i, len);
	tmp =(float) strtof(s, &endptr);
	if(*endptr != '\0' || endptr == s)
	{
		printf("Error in param alpha. %s is not a number\n", s);
		exit(1);
	}
	g->alpha = tmp;
	free(s);
	line = get_next_line(fd);
	//printf("we are here now: %s\n", line);
	//exit(0);
	i = 0;
	i = move_to_pos(line, pos);
	len = field_len(i, line);
	s = retrieve(line, i, len);
	tmp = (float) strtod(s, &endptr);
	if(*endptr != '\0' || endptr == s)
	{
		printf("Error in param beta. %s is not a number\n", s);
		exit(1);
	}
	g->beta = tmp;
	free(s);
	free(line);
}


void get_globals(char *filename, int old_fd, t_globals *g)
{
	int fd;
	int i;
	char *line;
	char *needle;
	int pos;

	close(old_fd);
	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr, "Error: cannot open file '%s'\n", filename);
		exit(1);
	}
	i = 0;
	while(1)
	{
		if(line == NULL)
		{
			printf("warning. Global variables not found\n");
			exit(1);
		}
		line = get_next_line(fd);
		needle = strstr(line, ",Corr Dupl,");
		if(needle)
		{
			pos = locate_pos(needle, line);
			load_globals(pos, fd, g);
			close(fd);
			free(line);
			return;
		}
		i++;
	}
}

void read_csv(char **args, t_channel **t, t_globals *g)
{
	int fd;
	int i;
	char *line;
	int pos;
	char *needle;

	fd = open(args[0], O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr, "Error: cannot open file '%s'\n", args[0]);
		exit(1);
	}

	i = 0;
	//line = get_next_line(fd);
	while(1)
	{
		if(line == NULL && i > 0) //fix it
		{
			printf("Warning. Parameters not found\n"); 
			exit(1);
		}
		line = get_next_line(fd);
		needle = strstr(line,",A,B,C,Nº,Channel,Inversión,% Inv,CPM Compra,"); // or any other admissible combinations of them
		if(needle)
		{
			pos = locate_pos(",Channel,",line);
			build_channels(t, fd, pos);
			fd = backtrack(i, args[0], fd);
			line = get_next_line(fd);
			//printf("we are now on %s\n", line);
			pos = locate_pos(needle, line);
			//printf("%i\n", pos);
			//exit(0);
			populate_channels(*t, fd, pos, needle);
			if(line)
				free(line);
			break;
		}
		free(line);
		i++;
	}
	get_globals(args[0],fd, g);
	//printf("%f %f\n", g->alpha, g->beta);
	//exit(0);
	get_universe(args, g);
}