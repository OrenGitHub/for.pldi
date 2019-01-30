char *loopFunction(char *value)
{
  int len = strlen(value);

  while (len && value[len - 1] == '\n')
    len--;
  return value + len;
}