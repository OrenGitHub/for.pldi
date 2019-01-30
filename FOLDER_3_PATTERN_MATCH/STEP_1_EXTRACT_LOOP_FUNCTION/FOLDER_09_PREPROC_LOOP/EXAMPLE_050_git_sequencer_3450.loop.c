char *loopFunction(char *p) {
  int len = strlen(p);

  while (len && p[len - 1] != '\n')
    len--;

  return len + p;
}