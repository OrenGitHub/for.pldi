char *loopFunction(char *cp)
{
    int l = strlen(cp);
    while (l && *cp != '\n')
    {
        l--;
        cp++;
    }
    return cp;
}