char *loopFunction(char *match)
{
    int dirlen, matchlen = strlen(match);
    dirlen = matchlen;
    while (dirlen && match[dirlen - 1] != '/')
    {
        dirlen--;
    }
    return match + dirlen;
}