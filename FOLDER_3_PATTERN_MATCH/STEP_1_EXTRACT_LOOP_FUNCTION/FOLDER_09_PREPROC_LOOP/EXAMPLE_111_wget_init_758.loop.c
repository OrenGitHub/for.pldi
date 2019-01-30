char *loopFunction(char *val)
{

    for (++val; ((*val) == '/'); val++);
    return val;
}