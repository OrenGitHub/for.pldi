char *loopFunction(char *str)
{

    char *s;
    for (s = str; (*s >= '0' && *s <= '9') || (*s >= 'a' && *s <= 'f'); s++)
                        ;
    return s;
}