char *loopFunction(char *expression)
{
    char *t;

    for (t = expression; (((*t) == ' ') || ((*t) == '\t')); t++);

    return t;
}