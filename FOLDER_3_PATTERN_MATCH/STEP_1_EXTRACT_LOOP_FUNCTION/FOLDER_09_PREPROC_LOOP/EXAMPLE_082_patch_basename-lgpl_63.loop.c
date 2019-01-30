char *loopFunction(char *name)
{

    int len;
    for (len = strlen(name); 1 < len && ((name[len - 1]) == '/' || (name[len - 1]) == '\\'); len--)
       continue;
    return name + len;
}