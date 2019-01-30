char *loopFunction(char *tmp)
{
    for (; *tmp == '\n' || *tmp == '\r'; tmp++)
    {
    }
    return tmp;
}