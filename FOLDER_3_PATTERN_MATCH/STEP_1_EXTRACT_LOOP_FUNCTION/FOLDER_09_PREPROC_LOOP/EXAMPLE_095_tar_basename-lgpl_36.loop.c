char *loopFunction(char *base)
{

    while (((*base) == '/' || (*base) == '\\'))
        base++;
    return base;
}