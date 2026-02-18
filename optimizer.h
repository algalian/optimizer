

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
#include <errno.h>
#include <stddef.h>


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
    long double min;
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
    int min;
    // add new fields here if struct grows
} t_colmap;
typedef struct globals
{
    long double alpha;
    long double beta;
    long double universe;
    long double acc;
    long double budget;
    int has_universo;
    int has_corr_dupl;
    int has_budget;
    unsigned long long iterations;
    long double max;
    long double max_snd;
} t_globals;

typedef struct s_ratio
{
	int	num;
	int	den;
}	t_ratio;

typedef int (*t_cmp) (const t_channel *a, const t_channel *b);

enum state { FIELD_START, IN_FIELD, IN_QUOTED, IN_QUOTED_ESC };


typedef enum e_col_type {
    COL_STRING,
    COL_LDOUBLE
} t_col_type;

typedef struct s_column_spec {
    const char *header_name;   /* Column name in file */
    t_col_type  type;          /* How to parse it */
    size_t      offset;        /* offsetof(t_channel, field) */
    bool        required;      /* Is column mandatory */
} t_column_spec;

void read_csv(char **args, t_channel **t, t_globals *g);
t_channel *logic_engine(t_channel **t, t_globals *g);
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
void append_node(t_channel **list, t_channel *node);
t_channel *new_node(void);
int load_channel_from_row(t_channel       *dst,
                          char           **cells,
                          int              row,
                          char           **header,
                          int              hcount,
                          const t_column_spec *schema);
int build_colmap(t_colmap *map, char **header, int hcount, char **fields);

/* ===================== PARSER INTERFACE ===================== */

typedef enum e_parse_error {
    PARSE_OK = 0,
    PARSE_EOF,
    PARSE_UNTERMINATED_QUOTE,
    PARSE_ALLOC_FAIL
} t_parse_error;

typedef enum e_sniff_status {
    SNIFF_OK = 0,
    SNIFF_ERR_OPEN,
    SNIFF_ERR_EMPTY,
    SNIFF_ERR_AMBIGUOUS
} t_sniff_status;

typedef struct s_parser {
    FILE *fp;
    char delimiter;
    const char *sheet_name;
    int (*open)(struct s_parser *, const char *);
    t_parse_error (*read_header)(struct s_parser *, char ***, int *);
    t_parse_error (*read_row)(struct s_parser *, char ***, int *);
    void (*close)(struct s_parser *);
    int (*write_cell)(struct s_parser *, int row, int col, const char *value);
    int (*save)(struct s_parser *);
} t_parser;

typedef struct s_corr_state {
    int col;
    int stage; /* 0 = inactive, 1 = read alpha next, 2 = read beta next */
} t_corr_state;

/* ===================== API ===================== */

int make_parser_for_file(const char *filename, t_parser *out);

t_parse_error read_delimited_record(FILE *fp,
                                    char delimiter,
                                    char ***out_cells,
                                    int *out_count);

int tokenize_csv_line(const char *buffer,
                      char delimiter,
                      char ***out_cells,
                      int *out_count,
                      int *out_in_quotes);
int tokenize_tsv_line(const char *buffer,
                      char ***out_cells,
                      int *out_count);

t_sniff_status sniff_delimiter(const char *path, char *out_delim);

void use_csv_parser(t_parser *p);
void use_tsv_parser(t_parser *p);

int load_channels_from_file(const char *filepath, char **fields, t_channel **list, t_globals *g, t_parser *p, t_colmap *map, int *row_num);


void free_cells(char **cells, int count);

int parse_long_double(const char *s,
                      long double *out,
                      const char *col_name);


void use_xlsx_parser(t_parser *p);



//-----------------write module

int write_in_file(const char *filename, t_channel *opt, t_globals *g, t_parser *p, t_colmap *map, int row_number);

#endif