char *loopFunction(char *path)
{
    int len = strlen(path);

    while ((1 < len) && (path[len - 1] == '/'))
        len--;
    return path + len;
}