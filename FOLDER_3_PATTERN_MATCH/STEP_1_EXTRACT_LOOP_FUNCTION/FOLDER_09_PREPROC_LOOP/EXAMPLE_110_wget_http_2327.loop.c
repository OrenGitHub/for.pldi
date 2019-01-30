char *loopFunction(char *cp)
{

    char *ep;
    for (ep = cp; *ep && *ep != '\"'; ep++)
        ;
    return ep;
}