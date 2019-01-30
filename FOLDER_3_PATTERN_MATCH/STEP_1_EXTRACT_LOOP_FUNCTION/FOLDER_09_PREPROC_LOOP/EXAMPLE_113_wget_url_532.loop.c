char *loopFunction(char *url)
{

    char *p;
    for (p = url; *p && *p != ':' && *p != '/'; p++);
    return p;
}