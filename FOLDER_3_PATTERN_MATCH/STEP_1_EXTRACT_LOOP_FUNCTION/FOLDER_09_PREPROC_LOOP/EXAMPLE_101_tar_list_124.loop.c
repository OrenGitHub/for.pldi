char *loopFunction(char *file_name)
{

    char* p;
    for (p = file_name; *p && (((*p) == '/' || (*p) == '\\') || *p == '.'); p++)
    {
        ;
    }
    return p;
}