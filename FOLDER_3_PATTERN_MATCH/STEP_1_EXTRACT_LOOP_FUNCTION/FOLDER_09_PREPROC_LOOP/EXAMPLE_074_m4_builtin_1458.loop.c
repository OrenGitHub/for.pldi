char *loopFunction(char *str)
{
    int len = strlen(str);
    int i;
    for (i = len; i > 1; i--)
        if (str[i - 1] != 'X')
            break;
    return str + i;
}