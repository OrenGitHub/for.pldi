char *loopFunction(char *myStatus)
{

    for (; *myStatus == ' ' || *myStatus == '\t'; myStatus++)
    {
        continue;
    }
    return myStatus;
}