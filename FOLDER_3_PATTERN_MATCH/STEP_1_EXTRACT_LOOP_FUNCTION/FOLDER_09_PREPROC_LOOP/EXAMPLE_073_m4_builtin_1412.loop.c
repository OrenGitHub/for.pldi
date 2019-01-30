char *loopFunction(char *pattern)
{

    int len = strlen(pattern);
    int i;
    for (i = 0; len > 0 && i < 6; i++)
        if (pattern[len - i - 1] != 'X')
          break;
    return pattern + i;
}