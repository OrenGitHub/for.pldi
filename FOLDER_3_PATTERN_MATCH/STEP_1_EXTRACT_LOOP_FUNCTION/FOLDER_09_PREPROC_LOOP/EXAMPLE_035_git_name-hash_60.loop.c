char *loopFunction(char *str)
{
    int namelen = strlen(str);
    while (namelen > 0 && !(((str[namelen - 1]) == '/') || ((str[namelen - 1]) == '\\')))
    {
        namelen--;
    }
    return str + namelen;
}