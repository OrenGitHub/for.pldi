char *loopFunction(char *tmp)
{
    for (; *tmp == '\t' || *tmp == ' '; tmp++)
    {
    }

    return tmp;
}