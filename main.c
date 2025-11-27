#include "optimizer.h"


int main(int argc, char **argv)
{
    t_channel *t;

    t = read_csv(argv[1]);
    logic_engine();
    free(t);
    return(0);
}