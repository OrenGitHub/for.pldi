char *loopFunction(char *prefix)
{
    int i = 0;

    while ((((prefix[i]) == '/') || ((prefix[i]) == '\\')))
    {
        i++;
    }
    return prefix + i;
}