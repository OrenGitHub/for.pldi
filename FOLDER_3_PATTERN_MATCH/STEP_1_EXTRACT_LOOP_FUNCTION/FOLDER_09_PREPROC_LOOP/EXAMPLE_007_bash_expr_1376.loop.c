char *loopFunction(char *xp)
{

  while (*xp && ((((*xp) == ' ') || ((*xp) == '\t')) || ((*xp) == '\n')))
  {
      xp++;
  }

  return xp;
}