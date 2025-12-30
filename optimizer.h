

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
# include <signal.h>
# include <stdint.h>
# include <time.h>
# include "libft/libft.h"
#include <inttypes.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif
# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif



typedef struct channel
{   
    char *name;
    long double a;
    long double b;
    long double c;
    long double cpm;
    int n;
    long double cob;
    long double not_cob;
    int inv;
    struct channel *next;
} t_channel;

typedef struct s_colmap {
    int name;
    int a;
    int b;
    int c;
    int cpm;
    int n;
    int cob;
    int not_cob;
    int inv;
    // add new fields here if struct grows
} t_colmap;
typedef struct globals
{
    long double alpha;
    long double beta;
    long double universe;
} t_globals;

typedef struct s_ratio
{
	int	num;
	int	den;
}	t_ratio;

typedef int (*t_cmp) (const t_channel *a, const t_channel *b);

enum state { FIELD_START, IN_FIELD, IN_QUOTED, IN_QUOTED_ESC };


void read_csv(char **args, t_channel **t, t_globals *g);
void logic_engine(t_channel **t, t_globals *g);
char	*get_next_line(int fd);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *str);
int locate_pos(char *needle, char *line);
void display_channels(const t_channel *head, const t_globals *g);
bool is_line_empty(char *s);
void merge_sort(t_channel **head_ref, t_cmp cmp);
void free_channels(t_channel *head);
int cmp_notcob_asc(const t_channel *a, const t_channel *b);
int cmp_n_asc(const t_channel *a, const t_channel *b);
void update_list(t_channel *dst, const t_channel*src);
t_channel *copy_list(const t_channel *src);
int count_channels(t_channel *t);
void write_csv();


/* =========================================================
** PARSER INTERFACE (file format abstraction layer)
** ========================================================= */

typedef struct s_parser 
{
    void    *ctx;   // Parser internal state (e.g., FILE*)
    char    delimiter;
    int     (*open)(struct s_parser *, const char *path);
    int     (*read_header)(struct s_parser *, char ***out_columns, int *out_count);
    int     (*read_row)(struct s_parser *, char ***out_cells, int *out_count);
    void    (*close)(struct s_parser *);
} t_parser;

/* -------- CSV parser selector -------- */
void    use_csv_parser(t_parser *p);

/* Helper for freeing rows/headers returned by read_row/read_header */
void    free_cells(char **cells, int count);
int load_channels_from_file(const char *filepath, char **fields, t_channel **list, t_globals *g);


int find_col(char **columns, int col_count, const char *name);


int tokenize_csv_line(const char *line, char delimiter, char ***out_cells, int *out_count);




void use_tsv_parser(t_parser *p);


t_parser make_parser_for_file(const char *filename);


#endif