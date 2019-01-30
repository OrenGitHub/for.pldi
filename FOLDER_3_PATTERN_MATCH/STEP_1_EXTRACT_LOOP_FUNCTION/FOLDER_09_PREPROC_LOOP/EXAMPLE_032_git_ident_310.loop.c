char *loopFunction(char *line)
{
    int len = strlen(line);
    char* cp;

    for (cp = line + len - 1; *cp != '>'; cp--)
        ;
    return cp;
}