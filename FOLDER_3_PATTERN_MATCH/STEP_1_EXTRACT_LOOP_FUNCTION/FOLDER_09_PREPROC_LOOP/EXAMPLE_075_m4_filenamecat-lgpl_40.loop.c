char *loopFunction(char *f)
{

    for (f += ((((unsigned int) ((f)[0]) | ('a' - 'A')) - 'a' <= 'z' - 'a') && (f)[1] == ':' ? 2 : 0); ((*f) == '/' || (*f) == '\\'); f++)
    {
        continue;
    }
    return f;
}