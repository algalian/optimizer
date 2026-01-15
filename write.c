#include "optimizer.h"
#include <xlsxwriter.h>

int write_in_file(const char *filename, t_channel *opt, t_globals *g, t_parser *p, t_colmap *map, int row_number)
{
	t_channel *current = opt;
	int i;
    lxw_workbook  *workbook  = workbook_new("results.xlsx");
    lxw_worksheet *sheet = workbook_add_worksheet(workbook, "Results");
	
	worksheet_write_string(sheet, 0, 0, "N", NULL);
	worksheet_write_string(sheet, 0, 1, "Inversión", NULL);
	i = 1;
	while(current)
	{
		//printf("%i %i\n", current->n, current->inv);
		worksheet_write_number(sheet, i, 0, current->n, NULL);
		worksheet_write_number(sheet, i, 1, current->inv, NULL);
		i++;
		current = current->next;
	}
	workbook_close(workbook);
	return(0);
}