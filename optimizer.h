

#ifndef OPTIMIZER_H
# define OPTIMIZER_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>
# include "libft/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif
# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif



typedef struct channel
{   
    char *name;
    double a;
    double b;
    double c;
    double cpm;
    int n;
    double cob;
    struct channel *next;
} t_channel;

typedef struct globals
{
    double alpha;
    double beta;
    double universe;
} t_globals;

void read_csv(char *filename, t_channel **t, t_globals *g);
void logic_engine(t_channel **t, t_globals *g);
char	*get_next_line(int fd);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *str);
int locate_pos(char *needle, char *line);
void display_channels(const t_channel *head, const t_globals *g);
bool is_line_empty(char *s);
void merge_sort(t_channel **head_ref);


#endif