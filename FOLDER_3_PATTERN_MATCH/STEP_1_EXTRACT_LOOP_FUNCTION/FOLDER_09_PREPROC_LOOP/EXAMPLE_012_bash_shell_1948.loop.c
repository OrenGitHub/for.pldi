char *loopFunction(char *s)
{
    while (*++s == '-');
    return s;
}