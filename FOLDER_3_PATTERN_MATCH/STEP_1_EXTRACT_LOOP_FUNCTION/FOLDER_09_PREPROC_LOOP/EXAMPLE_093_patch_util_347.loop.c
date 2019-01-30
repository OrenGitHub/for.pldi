char *loopFunction(char *t)
{

    char *o;
    int tlen = strlen(t);
    int olen;
    for (o = t + tlen, olen = 0; o > t && !((*(o - 1)) == '/' || (*(o - 1)) == '\\'); o--)
    {
                        ;
    }
    return o;
}