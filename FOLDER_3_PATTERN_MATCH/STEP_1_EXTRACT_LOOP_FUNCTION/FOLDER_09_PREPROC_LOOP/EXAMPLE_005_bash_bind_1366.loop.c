char *loopFunction(char *value)
{
    char *e = value + strlen(value) - 1;

    while (e >= value && (((*e) == ' ') || ((*e) == '\t')))
    {
        e--;
    }
    return e;
}