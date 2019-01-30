char *loopFunction(char *p)
{
    p += strlen(p) - 1;
    while (*p == '0')
    {
        p--;
    }
    return p;
}