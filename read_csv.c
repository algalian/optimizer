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



static void populate_name(t_channel *t, char *line, int field_len, int i)
{
	int j;
	t_channel *aux;

	aux = t;
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

static void populate_a(t_channel *t, char *line, int field_len, int i)
{
	int j;
	char *s;
	char *endptr;
	double temp;
	
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
	temp = strtod(s, &endptr);
	//printf("%s\n", s);
	if(*endptr != '\0' || endptr == line)
	{
		printf("%s is not a number. Discarded\n",s);
		free(s);
		return;
	}
	t->a = temp;
	printf("%f\n", t->a);
	free(s);
}

static void populate_channel(t_channel *t, int pos, char *line, int fd, char *field_name)
{
	int i;
	int commas;
	int k;
	int field_len;
	char *s;
	char *endptr;
	int j;
	double temp;
	int iter;

	commas = 0;
	iter = 0;
	while(t)
	{
		line = get_next_line(fd);
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
		field_len = 0;
		k = i;
		while(line[k] != ',' && line[k])
		{   
			field_len++;
			k++;
		}
		s = malloc(sizeof(char) * (field_len + 1));
		j = 0;
		while(j < field_len)
		{
			s[j] = line[i];
			j++;
			i++;
		}
		s[j] = '\0';
		temp = strtod(s, &endptr);
		if(*endptr != '\0' || endptr == line)
		{
			printf("%s is not a number. Discarded\n",s);
			free(s);
			return;
		}
		if(strcmp(field_name, "a") == 0)
			t->a = temp;
		if(strcmp(field_name, "b") == 0)
			t->b = temp;
		if(strcmp(field_name, "c") == 0)
			t->c = temp;
		if(strcmp(field_name, "cpm") == 0)
			t->cpm = temp;
		if(strcmp(field_name, "universo") == 0)
			t->universe = temp;
		t = t->next;
		free(line);
		iter++;
	}
}


void retrieve_field(int pos, t_channel *t, int fd, char *field)
{
	int i;
	int k;
	int iter;
	char *line;
	int commas;
	int field_len;
	//t_channel *aux;
	//static int init_struct;

	//aux = t;
	//display_channels(t);
	iter = 0;
	//printf("%i\n", init_struct);
	while(1)
	{
		line = get_next_line(fd);
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
			//printf("the loop was broken at %i i'm out\n", iter);
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
			populate_name(t, line, field_len, i);
		}
		t->next = malloc(sizeof(t_channel));
		if(!t->next)
		{
			perror("malloc error generating channel struct");
			exit(1);
		}
		memset(t->next, 0, sizeof(t_channel));
		/*printf("%s\n", aux->name);
		exit(0);*/
		t = t->next;
		t->next = NULL;
		free(line);
		iter++;
	}
	//display_channels(t);
}


void read_csv(char *filename, t_channel *t) 
{
	int fd;
	int i;
	char *line;
	char *needle;
	int pos;
	bool names;
	bool a;
	bool header;
	/*bool b;
	bool c;
	bool cpm;
	bool universe;*/

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr, "Error: cannot open file '%s'\n", filename);
		exit(1);
	}
	i = 0;
	names = false;
	a = false;
	header = false;
	t = malloc(sizeof(t_channel));
	if(!t)
	{
		perror("malloc error generating channel struct\n");
		exit(1);
	}
	while(1 && i < 20)
	{
		
		if (line == NULL || (names && a))
		{
			break;
		}
		if(header == false)
		{ 
			line = get_next_line(fd);
		}
		if(header == true)
		{    
			close(fd);
			fd = open(filename, O_RDONLY);
			if(fd < 0)
			{
				fprintf(stderr, "Error: cannot open file '%s'\n", filename);
				exit(1);
			}
			i = 0;
			header = false;
			line = get_next_line(fd);
		}

		if(!names) 
		{

			needle = strstr(line,",Channel,");
			if(needle)
			{
				pos = locate_pos(needle, line);
				retrieve_field(pos, t, fd, "name");
				names = true;
				close(fd);
				fd = open(filename, O_RDONLY);
				if(fd < 0)
				{
					fprintf(stderr, "Error: cannot open file '%s'\n", filename);
					exit(1);
				}
				i = 0;
				printf("displaying channels inside name retrieval...\n");
				display_channels(t);
				header = true;
				line = get_next_line(fd);
				printf("%s\n", line);
			}
		}
		if(!a)
		{
			//printf("entering in A check at %i\n", i);
			needle = strstr(line, ",A,");
			if(needle)
			{
				//printf("a field found at %i\n",i);
				pos = locate_pos(needle, line);
				populate_channel(t, pos, line, fd, "a");
				a = true;
				close(fd);
				fd = open(filename, O_RDONLY);
				if(fd < 0)
				{
					fprintf(stderr, "Error: cannot open file '%s'\n", filename);
					exit(1);
				}
				i = 0;
				printf("displaying channels inside A retrieval...\n");
				display_channels(t);
				header = true;
				line = get_next_line(fd);
			}
		}
		//printf("%i) %i, %i, %i\n%s\n", i, a, names, header, line);
		free(line);
		i++;
	}
	close(fd);
}