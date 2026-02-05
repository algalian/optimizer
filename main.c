#include "optimizer.h"

static int count_defaults(char **defaults)
{
    int i = 0;
    while (defaults[i]) i++;
    return i;
}

static void free_fields(char **fields)
{
    if (!fields) return;
    for (int i = 0; fields[i]; i++)
        free(fields[i]);
    free(fields);
}

static char **alloc_fields_from_defaults(char **defaults, int n)
{
    char **f = malloc(sizeof(char *) * (n + 1));
    if (!f)
        return NULL;

    for (int i = 0; i < n; i++)
        f[i] = strdup(defaults[i]);

    f[n] = NULL;
    return f;
}


static void print_menu(void)
{

	printf("====================================\n");
	printf(" Welcome to Budget Optimizer\n");
	printf("====================================\n");
	printf("1) Load default parameter pointers\n");
	printf("2) Modify parameter pointers\n");
	printf("3) Help\n");
	printf("4) Exit\n");
	printf("Enter your choice: ");
}

int get_menu_choice(void)
{
	char buffer[128];
	int choice;

	if (fgets(buffer, sizeof(buffer), stdin) == NULL) 
	{
		return -1;
	}
	buffer[strcspn(buffer, "\n")] = '\0';
	if (sscanf(buffer, "%d", &choice) != 1) 
	{
		return -1;
	}
	return choice;
}

static int listen_user_input(char **fields, char **defaults, int max_params)
{
    char buffer[128];

    printf("Modify the parameter names (hit Enter to use default)\n\n");

    for (int i = 0; i < max_params; i++)
    {
        if (i == 7)
            printf("Please enter the name of the sheet (default '%s'): ", defaults[i]);
        else
            printf("Param %d (default '%s'): ", i + 1, defaults[i]);

        if (!fgets(buffer, sizeof(buffer), stdin))
            return -1;

        buffer[strcspn(buffer, "\n")] = '\0';

       // free(fields[i]);  // ✅ important

        if (buffer[0] == '\0')
            fields[i] = strdup(defaults[i]);
        else
            fields[i] = strdup(buffer);

        if (!fields[i])
            return -1;
    }
    return 0;
}
static int get_input_path(char *input_path, size_t max_len)
{
    char buffer[2048];

	printf("Please enter the path to the file (if it's in this folder, just type the name)\n");
    if (!fgets(buffer, sizeof(buffer), stdin))
        return -1;
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(input_path, buffer, max_len - 1);
    input_path[max_len - 1] = '\0';
    return 0;
}

static void welcome_prompt(char ***fields, char **defaults,
                           char *help, char *input_path)
{
    int choice;
    int n = count_defaults(defaults);

    while (1)
    {
        print_menu();
        choice = get_menu_choice();

        switch (choice)
        {
            case 1:
                if (get_input_path(input_path, sizeof(char) * 2048) != 0)
                {
                    printf("Input file error\n");
                    exit(EXIT_FAILURE);
                }

                //free_fields(*fields);
                *fields = alloc_fields_from_defaults(defaults, n);

                if (!*fields)
                {
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                return;

            case 2:
                if (!*fields)
                {
                    printf("Load defaults first.\n\n");
                    break;
                }

                if (get_input_path(input_path, sizeof(char) * 2048) != 0)
                {
                    printf("Input file error\n");
                    exit(EXIT_FAILURE);
                }

                if (listen_user_input(*fields, defaults, n) != 0)
                {
                    printf("Input error\n");
                    exit(EXIT_FAILURE);
                }
                return;

            case 3:
                printf("%s\n\n", help);
                break;

            case 4:
                printf("Exiting program. Goodbye!\n");
                exit(EXIT_SUCCESS);

            default:
                printf("Invalid input. Please enter 1–4.\n\n");
        }
    }
}




int main(int argc, char **argv)
{
    char input_path[2048] = {0};
    char **fields = NULL;
    t_channel  *channels = NULL;
    t_channel  *opt = NULL;
    t_globals   globals = {0};
    int row_number = 0;
    t_parser p;
    t_colmap map;
    char *end;
    long double val;

    char *defaults[] = {
        "A",
        "B",
        "C",
        "CPM Compra",
        "Channel",
        "Corr Dupl",
        "Universo 000",
        "Simulador",
        "Inversión",
        NULL
    };

    if (argc > 3)
    {
        printf("Prompt error. Wrong number of args\n");
        return EXIT_FAILURE;
    }

    if (argc == 1)
    {
        welcome_prompt(&fields, defaults, ".help.txt", input_path);
    }
    else
    {
        strncpy(input_path, argv[1], sizeof(input_path) - 1);
        fields = alloc_fields_from_defaults(defaults,
                                            count_defaults(defaults));
    }

    if (argc == 3)
    {
        val = strtold(argv[2], &end);
        globals.acc = val;
    }
    else
    {
        globals.acc = 0.0055;
    }

    if (!fields)
    {
        fprintf(stderr, "Error: fields not initialized\n");
        return EXIT_FAILURE;
    }
    if (load_channels_from_file(input_path, fields,
                                &channels, &globals,
                                &p, &map, &row_number) != 0)
    {
        fprintf(stderr, "Error: failed to load channels from '%s'\n", input_path);
        free_fields(fields);
        free_channels(channels);
        return EXIT_FAILURE;
    }
	display_channels(channels,&globals);
    opt = logic_engine(&channels, &globals);
    if (write_in_file(input_path, opt, &globals, &p, &map, row_number) != 0)
    {
        fprintf(stderr, "Error: failed to write solution\n");
    }

    free_channels(channels);
    free_fields(fields);
    return EXIT_SUCCESS;
}