char *loopFunction(char *s)
{

    while (*s != '\n')
        s++;
    return s;
}