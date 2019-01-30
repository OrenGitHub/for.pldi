char *loopFunction(char *pbeg)
{
    while ((' ' == *pbeg) || ('\r' == *pbeg) || ('\n' == *pbeg) || ('\t' == *pbeg))
    {
        pbeg++;
    }
    return pbeg;
}