char *loopFunction(char *path)
{

    char *p = path + strlen(path);
    for (; *p != '/' && p != path; p--);
    return p;
}