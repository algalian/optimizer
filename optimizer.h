

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


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif
# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

typedef struct channel
{   
    char *name;
    int a;
    int b;
    int c;
    int cpm;
    int universe;
    struct channel *next;
} t_channel;

t_channel *read_csv(char *filename);
void logic_engine();
char	*get_next_line(int fd);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *str);


#endif