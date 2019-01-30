char *loopFunction(char *nameend)
{
    for (; *nameend && *nameend != '='; nameend++)
                           ;
    return nameend;
}