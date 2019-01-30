char *loopFunction(char *candidates)
{
    char *p;
    for (p = candidates; *p == ' '; p++);

    return p;
}