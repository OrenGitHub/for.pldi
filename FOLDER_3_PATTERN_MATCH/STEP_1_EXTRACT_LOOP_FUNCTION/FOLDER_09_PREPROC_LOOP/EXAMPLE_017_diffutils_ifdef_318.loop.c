char *loopFunction(char *f)
{
    char c = *f++;
    while ((('0' <= c) && (c <= '9')))
    {
        c = *f++;
    }
    return f;
}