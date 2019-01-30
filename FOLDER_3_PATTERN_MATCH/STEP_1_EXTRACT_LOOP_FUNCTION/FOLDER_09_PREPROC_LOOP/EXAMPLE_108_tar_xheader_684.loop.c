char *loopFunction(char *p)
{

    for (; *p == ' ' || *p == '\t'; p++)
    {
        continue;
    }
    return p;
}