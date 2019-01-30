char *loopFunction(char *path)
{


    int i = strlen(path);
    do
    {
        i--;
    }
    while (i > 0 && path[i] != '/');

    return path + i;
}