char *some_function(int n)
{
    n++;
    n++;
    n++;
    return(NULL);
}

void some_other_function(int n)
{
    printf("I want to preserve the value of n in this call of the function %i\n", n);
}

int main()
{
    int n;
    char *s;

    n = 0;
    s = some_function(&n);
    some_other_function(&n);
}