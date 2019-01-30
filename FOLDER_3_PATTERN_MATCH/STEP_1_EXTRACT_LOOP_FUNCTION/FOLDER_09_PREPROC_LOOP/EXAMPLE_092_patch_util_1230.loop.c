char *loopFunction(char *filename)
{

    char *f;
    for (f = filename + ((((unsigned int) ((filename)[0]) | ('a' - 'A')) - 'a' <= 'z' - 'a') && (filename)[1] == ':' ? 2 : 0); ((*f) == '/' || (*f) == '\\'); f++)
    {
                        ;
    }
    return f;
}