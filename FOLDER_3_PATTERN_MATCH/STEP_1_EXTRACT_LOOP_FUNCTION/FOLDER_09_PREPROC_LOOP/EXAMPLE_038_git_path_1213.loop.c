char *loopFunction(char *path)
{
    int len = strlen(path);

    while (len && (((path[len - 1]) == '/') || ((path[len - 1]) == '\\')))
    {
        len--;
    }
    return path + len;
}