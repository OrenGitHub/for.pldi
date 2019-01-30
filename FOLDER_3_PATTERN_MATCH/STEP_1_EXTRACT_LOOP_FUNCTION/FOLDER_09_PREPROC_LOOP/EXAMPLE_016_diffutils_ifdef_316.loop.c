char *loopFunction(char *f)
{
    char c;
    while ((c = *f++) == '-' || c == '\'' || c == '0')
    {
        continue;
    }
    return f;
}