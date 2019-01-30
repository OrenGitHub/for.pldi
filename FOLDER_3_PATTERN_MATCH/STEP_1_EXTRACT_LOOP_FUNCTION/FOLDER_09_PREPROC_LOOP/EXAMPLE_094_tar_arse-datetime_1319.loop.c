char *loopFunction(char *p)
{

    char c;
    while (c = *p, c_isspace(c))
        p++;
    return p;
}