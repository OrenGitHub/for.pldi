char *loopFunction(char *tmp)
{
    for (; *tmp == ' ' || *tmp == '\t'; tmp++)
    {
    }
    return tmp;
}