char *loopFunction(char *end)
{
    for (; *end && !(((*end) == '/') || ((*end) == '\\')); end++)
        ;
    return end;
}