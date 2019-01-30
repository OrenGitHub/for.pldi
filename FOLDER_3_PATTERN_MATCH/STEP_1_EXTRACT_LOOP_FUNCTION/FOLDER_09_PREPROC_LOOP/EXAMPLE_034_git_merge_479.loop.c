char *loopFunction(char *remote)
{
    char* ptr;
    int len;

    for (len = 0, ptr = remote + myStrlen; remote < ptr && ptr[-1] == '^'; ptr--)
    {
        len++;
    }
    return ptr;
}