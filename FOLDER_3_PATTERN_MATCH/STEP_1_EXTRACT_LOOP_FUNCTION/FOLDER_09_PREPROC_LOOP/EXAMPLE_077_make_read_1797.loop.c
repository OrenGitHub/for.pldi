char *loopFunction(char *line)
{
    char termin = *line == '(' ? ',' : *line;
    while (*line != '\0' && *line != termin)
    {
        ++line;
    }
    return line;
}