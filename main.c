#include "optimizer.h"


void free_channels(t_channel *head)
{
    t_channel *tmp;

    while (head)
    {
        tmp = head->next;   
        free(head);         
        head = tmp;         
    }
}



int main(int argc, char **argv)
{
    t_channel *t;
    //t_channel *tmp;
    t_globals g;


    t = NULL;
    read_csv(argv[1], &t, &g);
    //printf("File succesfully read and loaded. Displaying parameters...\n");
    //display_channels(t, &g);
    //printf("proceeding to calculate. Please wait...\n");
    logic_engine(&t, &g);
    if (stop_requested)
        printf("\nInterrupted! Checked %llu combinations.\n", (unsigned long long)checked);
    free_channels(t);
    return(0);
}