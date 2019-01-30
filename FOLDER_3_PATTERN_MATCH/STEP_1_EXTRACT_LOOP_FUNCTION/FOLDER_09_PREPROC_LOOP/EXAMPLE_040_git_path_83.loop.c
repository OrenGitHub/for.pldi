char *loopFunction(char *buf)
{
    int len = 0;
    while ((((buf[len]) == '/') || ((buf[len]) == '\\')))
    {
        len++;
    }

    return buf + len;
}