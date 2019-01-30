char *loopFunction(char *p)
{

    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
    {
        ++p;
    }
    return p;
}