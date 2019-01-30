char *loopFunction(char *url)
{
    int i;
    int url_len = strlen(url);
    for (i = url_len - 1; url[i] == '/' && 0 <= i; i--)
    {
        ;
    }

    return url + i;
}