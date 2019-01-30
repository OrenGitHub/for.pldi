char *loopFunction(char *in)
{
    int j = 0;
    while ((((in[j]) == '/') || ((in[j]) == '\\')))
    {
        j++;
    }
    return in + j;
}