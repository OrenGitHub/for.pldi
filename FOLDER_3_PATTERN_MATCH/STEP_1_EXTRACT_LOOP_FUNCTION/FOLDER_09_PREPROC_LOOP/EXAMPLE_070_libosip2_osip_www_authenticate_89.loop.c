char *loopFunction(char *hack)
{
    while (' ' == *(hack - 1))
    {
        hack--;
    }
    return hack;
}