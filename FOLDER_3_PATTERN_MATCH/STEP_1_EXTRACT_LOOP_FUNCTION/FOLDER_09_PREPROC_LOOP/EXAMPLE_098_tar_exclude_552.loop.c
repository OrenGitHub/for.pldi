char *loopFunction(char *pattern)
{

    int len = strlen(pattern);
    while (len > 0 && ((pattern[len - 1]) == '/' || (pattern[len - 1]) == '\\'))
    {
        --len;
    }
    return pattern + len;
}