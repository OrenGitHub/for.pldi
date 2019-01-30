char *loopFunction(char *p)
{

    while (*p == ' ' || *p == '\t')
        p++;
    return p;
}