char *loopFunction(char *p) {

  for (; ((*p) == '/' || (*p) == '\\'); p++)
    continue;
  return p;
}