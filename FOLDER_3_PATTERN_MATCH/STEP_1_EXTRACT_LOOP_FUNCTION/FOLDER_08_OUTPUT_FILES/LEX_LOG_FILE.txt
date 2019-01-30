char* loopFunction(char* line)
{
    char *p;

    for (p = line; p && *p && (((*p) == ' ') || ((*p) == '\t')); p++);


    return p;
}