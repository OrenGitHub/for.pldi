char *loopFunction(char *p)
{

    while (((unsigned) (*++p) - '0' <= 9))
                        ;
    return p;
}