char* loopFunction(char* n)
{



    char *se = n+strlen(n);
    while (n < se && *n != '/')
        ++n;

    return n;
}