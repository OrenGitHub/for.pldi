char *loopFunction(char *cp)
{
    char c;

    while ((c = *cp) && (((((c) == ' ') || ((c) == '\t')) || ((c) == '\n'))))
    {
        cp++;
    }
    return cp;
}